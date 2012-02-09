
#ifndef PHYSICSWORLD_H_Z8CVKYR9
#define PHYSICSWORLD_H_Z8CVKYR9

#define TICKS_PER_SECOND 120.0

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
	void removePlayer(Player* player);
	void addLinkSource(int id, PhysicsObject* obj);
	void addLinkSink(int id, PhysicsObject* obj);
	void addTeleporter(Ogre::String name, PhysicsObject* obj);
	
	virtual void step();
	virtual void sync();
	virtual WorldObjectState* save();
	virtual void restore(WorldObjectState* state);

	void init();

	dWorldID getWorld() { return mWorld; }
	dSpaceID getSpace() { return mSpace; }

	void nearCollide(dGeomID o1, dGeomID o2);

private:
	dWorldID mWorld;
	dSpaceID mSpace;
	dJointGroupID mContactGroup;

	Player* mPlayer1;
	Player* mPlayer2;
	GSList* mObjects;
	GSList* mSurfaces;
	GHashTable* mLinks; //int -> PhysicsLink
	GSList* mTeleports; //PhysicsTeleport
};

#endif

