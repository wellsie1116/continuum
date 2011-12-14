#ifndef PHYSICSWORLD_H_Z8CVKYR9
#define PHYSICSWORLD_H_Z8CVKYR9

#include "CompanionCube.h"
#include "TickTimer.h"

#include <Newton.h>

#include <glib.h>

class PhysicsWorld {
public:
	PhysicsWorld();
	virtual ~PhysicsWorld();

	CompanionCube* createCompanionCube(Ogre::SceneNode* node);

	void init();
	void start();
	void step();
	void stepWorld();

private:
	NewtonWorld* mWorld;

	TickTimer mTimer;
	GSList* mObjects;
};

#endif

