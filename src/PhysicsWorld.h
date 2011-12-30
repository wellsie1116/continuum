
#ifndef PHYSICSWORLD_H_Z8CVKYR9
#define PHYSICSWORLD_H_Z8CVKYR9

#include "Box.h"
#include "TickTimer.h"
#include "PhysicsState.h"

#include <ode/ode.h>

#include <glib.h>

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	Box* createCompanionCube(Ogre::SceneNode* node);
	Box* createBox(Ogre::SceneNode* node, float mass);
	Surface* createSurface(Ogre::SceneNode* node);

	int getTimestep();
	void init();
	void start();
	void step();
	void stepWorld();

	void freezeTime();
	void resumeTime();
	void accelerateTime();
	void decelerateTime();

	dWorldID getWorld() { return mWorld; }
	dSpaceID getSpace() { return mSpace; }

	void nearCollide(dGeomID o1, dGeomID o2);

private:
	void updateTimeRate();

private:
	dWorldID mWorld;
	dSpaceID mSpace;
	dJointGroupID mContactGroup;
	unsigned long mTimestep;
	int mStepRate;

	TickTimer mTimer;
	GSList* mObjects;
	GSList* mSurfaces;

	SnapshotManager mSnapshots;
};

#endif

