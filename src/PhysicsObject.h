
#ifndef PHYSICSOBJECT_H_L585YA8I
#define PHYSICSOBJECT_H_L585YA8I

#include <OgreSceneNode.h>

class PhysicsObjectState
{
public:
	virtual PhysicsObjectState* copy() const = 0;
};

class PhysicsObject
{
	public:
		virtual ~PhysicsObject() { }

		static Ogre::Vector3 getBounds(Ogre::SceneNode* node);

		virtual void setupForces() = 0;
		virtual void sync() = 0;

		virtual const PhysicsObjectState* save() const = 0;
		virtual void restore(const PhysicsObjectState* state) = 0;
};

#endif
