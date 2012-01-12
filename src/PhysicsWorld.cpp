
#include "PhysicsWorld.h"

#define TICKS_PER_SECOND 120.0

#define MAX_CONTACTS 50

static float MIN_WORLD_COORDS[] = {-5000.0f, -5000.0f, -5000.0f};
static float MAX_WORLD_COORDS[] = { 5000.0f,  5000.0f,  5000.0f};

static int DEFAULT_STEP_INDEX = 7;
static int FREEZE_STEP_INDEX = 5;
static float STEP_SPEEDS[] = {-16.0, -8.0, -4.0, -2.0, -1.0,
	0.0, 0.5, 1.0, 2.0, 4.0, 8.0, 16.0};

static void collideCallback(void *data, dGeomID o1, dGeomID o2);

PhysicsWorld::PhysicsWorld()
	: mWorld(NULL)
	, mSpace(NULL)
	, mContactGroup(NULL)
	, mObjects(NULL)
	, mSurfaces(NULL)
	, mTimer(TICKS_PER_SECOND)
	, mSnapshots(this)
{
}

PhysicsWorld::~PhysicsWorld()
{
	if (mWorld)
	{
		dWorldDestroy(mWorld);
		mWorld = NULL;
	}

	if (mSpace)
	{
		dSpaceDestroy(mSpace);
		mSpace = NULL;
	}
	
	if (mContactGroup)
	{
		dJointGroupDestroy(mContactGroup);
		mContactGroup = NULL;
	}

	if (mObjects)
	{
		g_slist_free_full(mObjects, (GDestroyNotify)physics_object_free);
		mObjects = NULL;
	}

	if (mSurfaces)
	{
		g_slist_free_full(mSurfaces, (GDestroyNotify)physics_object_free);
		mSurfaces = NULL;
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
	static bool init = false;
	if (!init)
	{
		dInitODE();
		init = true;
	}

	mWorld = dWorldCreate();
	mSpace = dHashSpaceCreate(0);
	dWorldSetGravity(mWorld, 0, -9.8, 0);
	mContactGroup = dJointGroupCreate(0);
}

void
PhysicsWorld::start()
{
	mTimestep = 0;
	mSnapshots.reset();
	mTimer.start();
}

void
PhysicsWorld::step()
{
	int ticks = mTimer.getTicks();

	if (ticks >= 0)
	{
		for (int i = 0; i < ticks; i++)
		{
			mTimestep++;
			stepWorld();
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
	GSList* pObjects = mObjects;
	while (pObjects)
	{
		PhysicsObject* obj = (PhysicsObject*)pObjects->data;
	
		obj->sync();
	
		pObjects = pObjects->next;
	}
}

void
PhysicsWorld::stepWorld()
{
	//setup forces
	for (GSList* pObjects = mObjects; pObjects; pObjects = pObjects->next)
	{
		PhysicsObject* obj = (PhysicsObject*)pObjects->data;
		obj->setupForces();
	}

	//step the world
	dSpaceCollide(mSpace, this, &collideCallback);
	dWorldQuickStep(mWorld, 1.0 / TICKS_PER_SECOND);
	dJointGroupEmpty(mContactGroup);
}

void
PhysicsWorld::freezeTime()
{
	mStepRate = FREEZE_STEP_INDEX;
	updateTimeRate();
}

void
PhysicsWorld::resumeTime()
{
	mStepRate = DEFAULT_STEP_INDEX;
	updateTimeRate();
}

void
PhysicsWorld::accelerateTime()
{
	mStepRate++;
	updateTimeRate();
}

void
PhysicsWorld::decelerateTime()
{
	mStepRate--;
	updateTimeRate();
}

void
PhysicsWorld::updateTimeRate()
{
	if (mStepRate < 0)
		mStepRate = 0;
	else if (mStepRate >= (sizeof(STEP_SPEEDS) / sizeof(STEP_SPEEDS[0])))
		mStepRate = (sizeof(STEP_SPEEDS) / sizeof(STEP_SPEEDS[0])) - 1;
	mTimer.setTickRate(STEP_SPEEDS[mStepRate]);
}

Box*
PhysicsWorld::createCompanionCube(Ogre::SceneNode* node)
{
	Box* cube = new CompanionCube(node, this);

	mObjects = g_slist_prepend(mObjects, cube);

	return cube;
}

Box*
PhysicsWorld::createBox(Ogre::SceneNode* node, float mass)
{
	Box* cube = new Box(node, this, mass);

	mObjects = g_slist_prepend(mObjects, cube);

	return cube;
}

Surface*
PhysicsWorld::createSurface(Ogre::SceneNode* node)
{
	Surface* surface = new Surface(node, this);

	mSurfaces = g_slist_prepend(mSurfaces, surface);

	return surface;
}

Player*
PhysicsWorld::createPlayer(Ogre::Camera* camera)
{
	Player* player = new Player(camera, this);
	
	mObjects = g_slist_prepend(mObjects, player);

	return player;
}

void
PhysicsWorld::nearCollide(dGeomID o1, dGeomID o2)
{
	int i;

	// Get the dynamics body for each geom
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	// Create an array of dContact objects to hold the contact joints
	dContact contact[MAX_CONTACTS];

	// Now we set the joint properties of each contact. Going into the full
	// details here would require a tutorial of its own. I'll just say that the
	// members of the dContact structure control the joint behaviour, such as
	// friction, velocity and bounciness. See section 7.3.7 of the ODE manual
	// and have fun experimenting to learn more.
	for (i = 0; i < MAX_CONTACTS; i++)
	{
		if (!b1 || !b2)
		{
			contact[i].surface.mode = 0;
			contact[i].surface.mu = 16.0;
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = 0.01;
			contact[i].surface.bounce_vel = 0.01;
			contact[i].surface.soft_cfm = 0.01;
		}
		else
		{
			contact[i].surface.mode = dContactBounce | dContactSoftCFM;
			contact[i].surface.mu = 16.0;
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = 0.01;
			contact[i].surface.bounce_vel = 0.01;
			contact[i].surface.soft_cfm = 0.01;
		}
	}

	// Here we do the actual collision test by calling dCollide. It returns the
	// number of actual contact points or zero if there were none.
	int numc;
	if ((numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))))
	{
		//To add each contact point found to our joint group we call
		//dJointCreateContact which is just one of the many different joint
		//types available.
		for (i = 0; i < numc; i++)
		{
			dJointID c = dJointCreateContact(mWorld, mContactGroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

static void
collideCallback(void *data, dGeomID o1, dGeomID o2)
{
	((PhysicsWorld*)data)->nearCollide(o1, o2);
}

