
#include "PhysicsWorld.h"

#define TICKS_PER_SECOND 120.0

#define SNAPSHOT_TICKS 100

#define MAX_CONTACTS 50

static float MIN_WORLD_COORDS[] = {-5000.0f, -5000.0f, -5000.0f};
static float MAX_WORLD_COORDS[] = { 5000.0f,  5000.0f,  5000.0f};

static void collideCallback(void *data, dGeomID o1, dGeomID o2);

PhysicsWorld::PhysicsWorld()
	: mWorld(NULL)
	, mSpace(NULL)
	, mContactGroup(NULL)
	, mObjects(NULL)
	, mSurfaces(NULL)
	, mTimer(TICKS_PER_SECOND)
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

	//FIXME free GSLists
}

int
PhysicsWorld::getTimestep()
{
	return mTimestep;
}

void
PhysicsWorld::init()
{
	//TODO only call this once
	dInitODE();

	mWorld = dWorldCreate();
	mSpace = dHashSpaceCreate(0);
	dWorldSetGravity(mWorld, 0, -9.8, 0);
	mContactGroup = dJointGroupCreate(0);
}

void
PhysicsWorld::start()
{
	mTimestep = 0;
	mTimer.start();

	startState = NULL;
}

void
PhysicsWorld::step()
{
	int ticks = mTimer.getTicks();

	for (int i = 0; i < ticks; i++)
	{
		mTimestep++;
		stepWorld();
	}

	//synchronize the world
	GSList* pObjects = mObjects;
	while (pObjects)
	{
		Box* cube = (Box*)pObjects->data;
	
		cube->sync();
	
		pObjects = pObjects->next;
	}
}

void
PhysicsWorld::stepWorld()
{
	if (mTimestep == 1200)
	{
		startState->restore();
		mTimestep = 300;
		return;
	}

	//step the world
	dRandSetSeed(0);
	dSpaceCollide(mSpace, this, &collideCallback);
	dWorldQuickStep(mWorld, 1.0 / TICKS_PER_SECOND);
	dJointGroupEmpty(mContactGroup);
	
	if (mTimestep % SNAPSHOT_TICKS == SNAPSHOT_TICKS - 1)
	{
		//TODO save the world state
		//WorldSnapshot snapshot(this);
		//NewtonInvalidateCache(mWorld);
	}
	//WorldSnapshot snapshot(this);
	//snapshot.restore();

	if (mTimestep == 300 && !startState)
	{
		startState = new WorldSnapshot(this);
	}

}

Box*
PhysicsWorld::createCompanionCube(Ogre::SceneNode* node)
{
	Box* cube = new CompanionCube(node, this);

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
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = 2.0;
		contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 0.01;
		contact[i].surface.bounce_vel = 0.1;
		contact[i].surface.soft_cfm = 0.01;
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

WorldSnapshot::WorldSnapshot(PhysicsWorld* world)
	: mWorld(world)
{
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
		
		vect = dBodyGetForce(mBody);
		for (int i = 0; i < 3; i++)
			mForce[i] = vect[i];
		
		vect = dBodyGetTorque(mBody);
		for (int i = 0; i < 3; i++)
			mTorque[i] = vect[i];
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
		dBodySetForce(mBody, mForce[0], mForce[1], mForce[2]);
		dBodySetTorque(mBody, mTorque[0], mTorque[1], mTorque[2]);
	}
}

PhysicsObject*
BodyState::getPhysObject()
{
	return (PhysicsObject*)dGeomGetData(mGeom);
}

