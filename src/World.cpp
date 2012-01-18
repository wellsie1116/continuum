
#include "World.h"

static int DEFAULT_STEP_INDEX = 7;
static int FREEZE_STEP_INDEX = 5;
static float STEP_SPEEDS[] = {-16.0, -8.0, -4.0, -2.0, -1.0,
	0.0, 0.5, 1.0, 2.0, 4.0, 8.0, 16.0};

static void free_state(WorldObjectState* state, gpointer user_data);
	
World::World()
	: mTimer(TICKS_PER_SECOND)
	, mStepRate(DEFAULT_STEP_INDEX)
	, mTimestep(0)
	, mObjects(NULL)
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
			for (GList* pObjects = mObjects->head; pObjects; pObjects = pObjects->next)
			{
				WorldObject* pObj = (WorldObject*)pObjects->data;
				pObj->step();
			}
			//TODO save snapshot?
			//mSnapshots.worldTick(mTimestep);
		}
	}
	else
	{
		//TODO implement
		//ticks = -ticks;
		//if (ticks > mTimestep)
		//	mTimestep = 0;
		//else
		//	mTimestep -= ticks;

		//mSnapshots.restoreSnapshot(mTimestep);
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

static void
free_state(WorldObjectState* state, gpointer user_data)
{
	delete state;
}
