
#include "PhysicsWorld.h"

#define TICKS_PER_SECOND 120.0

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


void
PhysicsWorld::init()
{
	// create the Newton World
	mWorld = NewtonCreate();
 
	// use the standard x87 floating point model  
	NewtonSetPlatformArchitecture(mWorld, 0);
 
	// set a fix world size
	NewtonSetWorldSize(mWorld, MIN_WORLD_COORDS, MAX_WORLD_COORDS);
 
	// configure the Newton world to use iterative solve mode 0
	// this is the most efficient but the less accurate mode
	NewtonSetSolverModel(mWorld, 1);
}

void
PhysicsWorld::start()
{
	mTimer.start();
}

void
PhysicsWorld::step()
{
	int ticks = mTimer.getTicks();

	for (int i = 0; i < ticks; i++)
	{
		stepWorld();
	}

	//synchronize the world
	GSList* pObjects = mObjects;
	while (pObjects)
	{
		CompanionCube* cube = (CompanionCube*)pObjects->data;
	
		cube->sync();
	
		pObjects = pObjects->next;
	}
}

void
PhysicsWorld::stepWorld()
{
	NewtonUpdate(mWorld, 1.0 / TICKS_PER_SECOND);
}

CompanionCube*
PhysicsWorld::createCompanionCube(Ogre::SceneNode* node)
{
	CompanionCube* cube = new CompanionCube(node, mWorld);

	mObjects = g_slist_prepend(mObjects, cube);

	return cube;
}

