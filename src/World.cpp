
#include "World.h"

#define SNAPSHOT_TICKS 100
	
static int DEFAULT_STEP_INDEX = 7;
static int FREEZE_STEP_INDEX = 5;
static float STEP_SPEEDS[] = {-16.0, -8.0, -4.0, -2.0, -1.0,
	0.0, 0.5, 1.0, 2.0, 4.0, 8.0, 16.0};

World::World()
	: mTimer(TICKS_PER_SECOND)
	, mStepRate(DEFAULT_STEP_INDEX)
	, mTimestep(0)
	, mObjects(NULL)
	, mSnapshots(this)
{
	mObjects = g_queue_new();
}

World::~World()
{
	//g_queue_foreach(mObjects, (GFunc)free_state, NULL);
	g_queue_free(mObjects);
}
	
void
World::addObject(WorldObject* object)
{
	g_queue_push_tail(mObjects, object);
}
	
void
World::start()
{
	mStepRate = DEFAULT_STEP_INDEX;
	mTimestep = 0;
	mSnapshots.reset();
	mTimer.start();
}

void
World::freezeTime()
{
	mStepRate = FREEZE_STEP_INDEX;
	updateTimeRate();
}

void
World::resumeTime()
{
	mStepRate = DEFAULT_STEP_INDEX;
	updateTimeRate();
}

void
World::accelerateTime()
{
	mStepRate++;
	updateTimeRate();
}

void
World::decelerateTime()
{
	mStepRate--;
	updateTimeRate();
}

void
World::step()
{
	int ticks = mTimer.getTicks();

	if (ticks >= 0)
	{
		for (int i = 0; i < ticks; i++)
		{
			mTimestep++;
			stepOnce();
			mSnapshots.worldTick(mTimestep);
		}
	}
	else
	{
		ticks = -ticks;
		if (ticks > mTimestep)
			mTimestep = 0;
		else
			mTimestep -= ticks;

		mSnapshots.restoreSnapshot(mTimestep);
	}

	//synchronize the world
	for (GList* pObjects = mObjects->head; pObjects; pObjects = pObjects->next)
	{
		WorldObject* pObj = (WorldObject*)pObjects->data;
		pObj->sync();
	}
}



void
World::updateTimeRate()
{
	if (mStepRate < 0)
		mStepRate = 0;
	else if (mStepRate >= (sizeof(STEP_SPEEDS) / sizeof(STEP_SPEEDS[0])))
		mStepRate = (sizeof(STEP_SPEEDS) / sizeof(STEP_SPEEDS[0])) - 1;
	mTimer.setTickRate(STEP_SPEEDS[mStepRate]);
}
	
void
World::stepOnce()
{
	for (GList* pObjects = mObjects->head; pObjects; pObjects = pObjects->next)
	{
		WorldObject* pObj = (WorldObject*)pObjects->data;
		pObj->step();
	}
}

World::Snapshot::Snapshot(World* world, int timestep)
	: mTimestep(timestep)
{
	mCount = world->mObjects->length;
	mObjects = new WorldObject*[mCount];
	mStates = new WorldObjectState*[mCount];

	int i = 0;
	for (GList* pObjects = world->mObjects->head; pObjects; pObjects = pObjects->next)
	{
		WorldObject* pObj = (WorldObject*)pObjects->data;
		WorldObjectState* state = pObj->save();

		mObjects[i] = pObj;
		mStates[i] = state;
		i++;
	}
}

World::Snapshot::~Snapshot()
{
	for (int i = 0; i < mCount; i++)
		if (mStates[i])
			delete mStates[i];

	delete [] mObjects;
	delete [] mStates;
}

void
World::Snapshot::restore()
{
	for (int i = 0; i < mCount; i++)
	{
		mObjects[i]->restore(mStates[i]);
	}
}

void
World::Snapshot::free(World::Snapshot* snapshot, gpointer user_data)
{
	delete snapshot;
}


World::SnapshotManager::SnapshotManager(World* world)
	: mWorld(world)
{
	mSnapshots = g_queue_new();
}

World::SnapshotManager::~SnapshotManager()
{
	g_queue_foreach(mSnapshots, (GFunc)World::Snapshot::free, NULL);
	g_queue_free(mSnapshots);
}

void
World::SnapshotManager::reset()
{
	//clear old states
	g_queue_foreach(mSnapshots, (GFunc)World::Snapshot::free, NULL);
	g_queue_clear(mSnapshots);

	//add the initial state
	add(new World::Snapshot(mWorld, 0));
}

void
World::SnapshotManager::worldTick(int timestep)
{
	if (timestep % SNAPSHOT_TICKS == SNAPSHOT_TICKS - 1)
	{
		World::Snapshot* snapshot = new World::Snapshot(mWorld, timestep);
		add(snapshot);
	}
}

void
World::SnapshotManager::restoreSnapshot(int timestep)
{
	World::Snapshot* snapshot = getClosest(timestep);
	purgeAfter(snapshot->getTimestep());
	snapshot->restore();

	for (int i = snapshot->getTimestep() + 1; i <= timestep; i++)
	{
		mWorld->stepOnce();
		snapshot = new World::Snapshot(mWorld, i);
		add(snapshot);
	}
}

void
World::SnapshotManager::add(World::Snapshot* snapshot)
{
	if (mSnapshots->length)
	{
		g_assert_cmpint(
				((World::Snapshot*)mSnapshots->tail->data)->getTimestep(),
				<,
				snapshot->getTimestep());
	}

	g_queue_push_tail(mSnapshots, snapshot);
}

World::Snapshot*
World::SnapshotManager::getClosest(int timestep)
{
	for (GList* pStates = mSnapshots->tail; pStates; pStates = pStates->prev)
	{
		World::Snapshot* snapshot = (World::Snapshot*)pStates->data;
		if (snapshot->getTimestep() < timestep)
			return snapshot;
	}
	return (World::Snapshot*)mSnapshots->head->data;
}

void
World::SnapshotManager::purgeAfter(int timestep)
{
	if (timestep < 0)
		timestep = 0;

	World::Snapshot* snapshot;
	while (snapshot = (World::Snapshot*)g_queue_peek_tail(mSnapshots))
	{
		if (snapshot->getTimestep() <= timestep)
			break;

		World::Snapshot::free(snapshot, NULL);
		g_queue_pop_tail(mSnapshots);
	}
}

