
#ifndef BOX_H_QNH37BLD
#define BOX_H_QNH37BLD

#include <OgreSceneNode.h>

#include <ode/ode.h>

class PhysicsWorld;

class PhysicsObject
{
	public:
		static Ogre::Vector3 getBounds(Ogre::SceneNode* node);

		virtual void sync() = 0;
};

class Box : public PhysicsObject
{
	public:
		Box(Ogre::SceneNode* node, PhysicsWorld* world, float mass);
		virtual ~Box();
		
		virtual void sync();

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
		
		virtual void sync();

	protected:
		Ogre::SceneNode* mNode;
		dGeomID mGeom;
};

#endif

