
#include "PhysicsState.h"

#include "PhysicsWorld.h"

#define SNAPSHOT_TICKS 100

WorldSnapshot::WorldSnapshot(PhysicsWorld* world, int timestep)
	: mWorld(world)
	, mTimestep(timestep)
{
	mSeed = dRandGetSeed();

	dSpaceID space = world->getSpace();
	mBodyCount = dSpaceGetNumGeoms(space);
	mStates = new BodyState*[mBodyCount];

	for (int i = 0; i < mBodyCount; i++)
	{
		mStates[i] = new BodyState(dSpaceGetGeom(space, i));
	}
}

WorldSnapshot::~WorldSnapshot()
{
	if (mStates)
	{
		for (int i = 0; i < mBodyCount; i++)
		{
			delete mStates[i];
		}
		delete mStates;
		mStates = NULL;
	}
}

void
WorldSnapshot::restore()
{
	for (int i = 0; i < mBodyCount; i++)
	{
		PhysicsObject* obj = mStates[i]->getPhysObject();
		mStates[i]->restore();
		obj->sync();
	}

	dRandSetSeed(mSeed);
}

BodyState::BodyState(dGeomID geom)
	: mGeom(geom)
{
	dGeomCopyPosition(mGeom, mPosition);
	dGeomCopyRotation(mGeom, mOrientation);

	mBody = dGeomGetBody(mGeom);
	if (mBody)
	{
		const float* vect;

		vect = dBodyGetLinearVel(mBody);
		for (int i = 0; i < 3; i++)
			mLinearVelocity[i] = vect[i];

		vect = dBodyGetAngularVel(mBody);
		for (int i = 0; i < 3; i++)
			mAngularVelocity[i] = vect[i];
		
		//vect = dBodyGetForce(mBody);
		//for (int i = 0; i < 3; i++)
		//	mForce[i] = vect[i];
		//
		//vect = dBodyGetTorque(mBody);
		//for (int i = 0; i < 3; i++)
		//	mTorque[i] = vect[i];
	}
}

void
BodyState::restore()
{
	dGeomSetPosition(mGeom, mPosition[0], mPosition[1], mPosition[2]);
	dGeomSetRotation(mGeom, mOrientation);

	if (mBody)
	{
		dBodySetLinearVel(mBody, mLinearVelocity[0], mLinearVelocity[1], mLinearVelocity[2]);
		dBodySetAngularVel(mBody, mAngularVelocity[0], mAngularVelocity[1], mAngularVelocity[2]);
		//dBodySetForce(mBody, mForce[0], mForce[1], mForce[2]);
		//dBodySetTorque(mBody, mTorque[0], mTorque[1], mTorque[2]);
	}
}

PhysicsObject*
BodyState::getPhysObject()
{
	return (PhysicsObject*)dGeomGetData(mGeom);
}

SnapshotManager::SnapshotManager(PhysicsWorld* world)
	: mWorld(world)
	, mStartState(NULL)
{
	mStates = g_queue_new();
}

SnapshotManager::~SnapshotManager()
{
	//TODO cleanup
}

void
SnapshotManager::reset()
{
	if (mStartState)
	{
		delete mStartState;
		mStartState = NULL;
	}

	//TODO clear old states

	mStartState = new WorldSnapshot(mWorld, 0);
	g_queue_push_tail(mStates, mStartState);
	mStartState = new WorldSnapshot(mWorld, 0);
}

void
SnapshotManager::worldTick(int timestep)
{
	if (timestep % SNAPSHOT_TICKS == SNAPSHOT_TICKS - 1)
	{
		WorldSnapshot* state = new WorldSnapshot(mWorld, timestep);

		g_queue_push_tail(mStates, state);
	}
}

void
SnapshotManager::restoreSnapshot(int timestep)
{
	if (timestep <= 0)
	{
		WorldSnapshot* state;
		while (state = (WorldSnapshot*)g_queue_pop_tail(mStates))
		{
			delete state;
		}
		mStartState->restore();
	}

	WorldSnapshot* state;
	while (state = (WorldSnapshot*)g_queue_peek_tail(mStates))
	{
		if (state->getTimestep() == timestep)
		{
			state->restore();
			return;
		}
		else if (state->getTimestep() < timestep)
		{
			state->restore();
			for (int i = state->getTimestep() + 1; i <= timestep; i++)
			{
				mWorld->stepWorld();
				state = new WorldSnapshot(mWorld, i);
				g_queue_push_tail(mStates, state);
			}
			return;
		}
		else
		{
			delete state;
			g_queue_pop_tail(mStates);
		}
	}
}

