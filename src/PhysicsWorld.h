
#ifndef PHYSICSWORLD_H_Z8CVKYR9
#define PHYSICSWORLD_H_Z8CVKYR9

#include "Box.h"
#include "TickTimer.h"

#include <ode/ode.h>

#include <glib.h>

class BodyState
{
public:
	BodyState(dGeomID geom);

	void restore();
	PhysicsObject* getPhysObject();

private:
	dGeomID mGeom;
	dBodyID mBody;

	dVector3 mPosition;
	dMatrix3 mOrientation;
	float mLinearVelocity[3];
	float mAngularVelocity[3];
	float mForce[3];
	float mTorque[3];
};

class WorldSnapshot
{
public:
	WorldSnapshot(PhysicsWorld* world);
	~WorldSnapshot();

	void restore();

private:
	PhysicsWorld* mWorld;
	BodyState** mStates;
	int mBodyCount;
	unsigned long mSeed;
};

class PhysicsWorld
{
public:
	PhysicsWorld();
	virtual ~PhysicsWorld();

	Box* createCompanionCube(Ogre::SceneNode* node);
	Surface* createSurface(Ogre::SceneNode* node);

	int getTimestep();
	void init();
	void start();
	void step();

	void freezeTime();
	void resumeTime();
	void accelerateTime();
	void decelerateTime();

	dWorldID getWorld() { return mWorld; }
	dSpaceID getSpace() { return mSpace; }

	void nearCollide(dGeomID o1, dGeomID o2);

private:
	void stepWorld();

private:
	dWorldID mWorld;
	dSpaceID mSpace;
	dJointGroupID mContactGroup;
	unsigned long mTimestep;

	TickTimer mTimer;
	GSList* mObjects;
	GSList* mSurfaces;

	WorldSnapshot* startState;
};

#endif

