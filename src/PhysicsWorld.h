
#ifndef PHYSICSWORLD_H_Z8CVKYR9
#define PHYSICSWORLD_H_Z8CVKYR9

#include "WorldObject.h"
#include "Box.h"
#include "Player.h"

#include <ode/ode.h>

#include <glib.h>

class PhysicsWorld : public WorldObject
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	Box* createCompanionCube(Ogre::SceneNode* node);
	Box* createBox(Ogre::SceneNode* node, float mass);
	Surface* createSurface(Ogre::SceneNode* node);
	Player* createPlayer(Ogre::Camera* camera);
	
	virtual void step();
	virtual void sync();
	virtual const WorldObjectState* save() const { return NULL; }
	virtual void restore(const WorldObjectState* state) { }

	void init();

	dWorldID getWorld() { return mWorld; }
	dSpaceID getSpace() { return mSpace; }

	void nearCollide(dGeomID o1, dGeomID o2);

private:
	dWorldID mWorld;
	dSpaceID mSpace;
	dJointGroupID mContactGroup;

	GSList* mObjects;
	GSList* mSurfaces;
};

#endif

