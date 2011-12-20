
#include "PhysicsWorld.h"

#define TICKS_PER_SECOND 120.0

#define SNAPSHOT_TICKS 100

static float MIN_WORLD_COORDS[] = {-5000.0f, -5000.0f, -5000.0f};
static float MAX_WORLD_COORDS[] = { 5000.0f,  5000.0f,  5000.0f};

PhysicsWorld::PhysicsWorld()
	: mWorld(NULL)
	, mObjects(NULL)
	, mTimer(TICKS_PER_SECOND)
{
}

PhysicsWorld::~PhysicsWorld()
{
	if (mWorld)
	{
		NewtonDestroy(mWorld);
		mWorld = NULL;
	}
}

int
PhysicsWorld::getTimestep()
{
	return mTimestep;
}

void
PhysicsWorld::init()
{
	// create the Newton World
	mWorld = NewtonCreate();

	// keep things deterministic
	NewtonSetThreadsCount(mWorld, 1);
 
	// use the standard x87 floating point model  
	NewtonSetPlatformArchitecture(mWorld, 0);
 
	// set a fix world size
	NewtonSetWorldSize(mWorld, MIN_WORLD_COORDS, MAX_WORLD_COORDS);
 
	// configure the Newton world to use iterative solve mode 0
	// this is the most efficient but the less accurate mode
	NewtonSetSolverModel(mWorld, 1);

	NewtonWorldSetUserData(mWorld, this);
}

void
PhysicsWorld::start()
{
	mTimestep = 0;
	mTimer.start();

	NewtonInvalidateCache(mWorld);
	startState = NULL;
}

void
PhysicsWorld::step()
{
	int ticks = mTimer.getTicks();

	for (int i = 0; i < ticks; i++)
	{
		stepWorld();
		mTimestep++;
	}

	//synchronize the world
	//GSList* pObjects = mObjects;
	//while (pObjects)
	//{
	//	Box* cube = (Box*)pObjects->data;
	//
	//	cube->sync();
	//
	//	pObjects = pObjects->next;
	//}
}

void
PhysicsWorld::stepWorld()
{
	NewtonUpdate(mWorld, 1.0 / TICKS_PER_SECOND);
	
	if (mTimestep % SNAPSHOT_TICKS == SNAPSHOT_TICKS - 1)
	{
		//TODO save the world state
		WorldSnapshot snapshot(mWorld);
		NewtonInvalidateCache(mWorld);
	}
	//WorldSnapshot snapshot(mWorld);
	//snapshot.restore();

	if (mTimestep == 300 && !startState)
	{
		NewtonInvalidateCache(mWorld);
		startState = new WorldSnapshot(mWorld);
	}

	if (mTimestep == 1200)
	{
		startState->restore();
		NewtonInvalidateCache(mWorld);
		mTimestep = 300;
	}
}

Box*
PhysicsWorld::createCompanionCube(Ogre::SceneNode* node)
{
	Box* cube = new Box(node, mWorld, 5.0f);

	mObjects = g_slist_prepend(mObjects, cube);

	return cube;
}

Box*
PhysicsWorld::createSurface(Ogre::SceneNode* node)
{
	Box* cube = new Surface(node, mWorld);

	mObjects = g_slist_prepend(mObjects, cube);

	return cube;
}
	
WorldSnapshot::WorldSnapshot(NewtonWorld* world)
	: mWorld(world)
{
	mBodyCount = NewtonWorldGetBodyCount(world);
	mStates = new BodyState*[mBodyCount];

	NewtonBody* body = NewtonWorldGetFirstBody(mWorld);
	for (int i = 0; body; i++)
	{
		mStates[i] = new BodyState(body);
		body = NewtonWorldGetNextBody(mWorld, body);
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
		Box* box = mStates[i]->getPhysObject();
		mStates[i]->restore();
		box->sync();
	}
}

BodyState::BodyState(NewtonBody* body)
	: mBody(body)
{
	NewtonBodyGetVelocity(mBody, mVelocity);
	NewtonBodyGetOmega(mBody, mOmega);
	NewtonBodyGetMatrix(mBody, mTransform);

	//NewtonJoint* joint = NewtonBodyGetFirstContactJoint(mBody);
	//for (int i = 0; joint; i++)
	//{
	//	NewtonJointRecord record;
	//	NewtonJointGetInfo(joint, &record);
	//	joint = NewtonBodyGetNextContactJoint(mBody, joint);
	//}

}

void
BodyState::restore()
{
	NewtonBodySetMatrix(mBody, mTransform);
	NewtonBodySetVelocity(mBody, mVelocity);
	NewtonBodySetOmega(mBody, mOmega);
}

Box*
BodyState::getPhysObject()
{
	return (Box*)NewtonBodyGetUserData(mBody);
}

