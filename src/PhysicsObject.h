
#ifndef PHYSICSOBJECT_H_L585YA8I
#define PHYSICSOBJECT_H_L585YA8I

#include <OgreSceneNode.h>

class PhysicsObject
{
	public:
		virtual ~PhysicsObject() { }

		static Ogre::Vector3 getBounds(Ogre::SceneNode* node);

		virtual void sync() = 0;
};

#endif
