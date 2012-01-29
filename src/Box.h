
#ifndef BOX_H_QNH37BLD
#define BOX_H_QNH37BLD

#include <OgreSceneNode.h>

#include <ode/ode.h>

#include "PhysicsObject.h"

class PhysicsWorld;

class Box : public PhysicsObject
{
	public:
		Box(Ogre::SceneNode* node, PhysicsWorld* world, float mass);
		virtual ~Box();
		
		virtual void setupForces() {}
		virtual void sync();
		virtual const PhysicsObjectState* save() const { return NULL; }
		virtual void restore(const PhysicsObjectState* state) {}

	protected:
		Ogre::SceneNode* mNode;
		dBodyID mBody;
		dGeomID mGeom;
		const float* mPos;
		float mMass;
};

class CompanionCube : public Box
{
	public:
		CompanionCube(Ogre::SceneNode* node, PhysicsWorld* world)
			: Box(node, world, 5.0f)
		{ }
};


class Surface : public PhysicsObject
{
	public:
		Surface(Ogre::SceneNode* node, PhysicsWorld* world);
		virtual ~Surface();
		
		virtual void linkToggled();
		virtual bool contains(dGeomID geom);
		virtual void setupForces();
		virtual void sync();
		virtual const PhysicsObjectState* save() const { return NULL; }
		virtual void restore(const PhysicsObjectState* state) {}

	protected:
		Ogre::SceneNode* mNode;
		dGeomID mGeom;
		const float* mPos;
};

void physics_object_free(PhysicsObject* object);

#endif

