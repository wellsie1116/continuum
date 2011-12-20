
#ifndef PHYSICSWORLD_H_Z8CVKYR9
#define PHYSICSWORLD_H_Z8CVKYR9

#include "Box.h"
#include "TickTimer.h"

#include <Newton.h>

#include <glib.h>

class BodyState
{
public:
	BodyState(NewtonBody* body);

	void restore();
	Box* getPhysObject();

private:
	NewtonBody* mBody;
	float mTransform[16];
	float mVelocity[3];
	float mOmega[3];
};

class WorldSnapshot
{
public:
	WorldSnapshot(NewtonWorld* world);
	~WorldSnapshot();

	void restore();

private:
	NewtonWorld* mWorld;
	BodyState** mStates;
	int mBodyCount;
};

class PhysicsWorld
{
public:
	PhysicsWorld();
	virtual ~PhysicsWorld();

	Box* createCompanionCube(Ogre::SceneNode* node);
	Box* createSurface(Ogre::SceneNode* node);

	int getTimestep();
	void init();
	void start();
	void step();


private:
	void stepWorld();

private:
	NewtonWorld* mWorld;
	unsigned long mTimestep;

	TickTimer mTimer;
	GSList* mObjects;

	WorldSnapshot* startState;
};

#endif

