
#ifndef PHYSICSOBJECT_H_L585YA8I
#define PHYSICSOBJECT_H_L585YA8I

#include <OgreSceneNode.h>

#include <ode/ode.h>

class PhysicsObjectState
{
public:
	virtual PhysicsObjectState* copy() const = 0;
};

class PhysicsObject
{
	public:
		virtual ~PhysicsObject() { }

		static Ogre::Vector3 getBounds(Ogre::SceneNode* node, Ogre::Vector3& offset);
		
		void setLinkEnabled(bool enabled);
		virtual void linkToggled() {}

		virtual bool contains(dGeomID geom);

		virtual void setupForces() = 0;
		virtual void sync() = 0;

		virtual const PhysicsObjectState* save() const = 0;
		virtual void restore(const PhysicsObjectState* state) = 0;

	protected:
		bool mLinkEnabled;
};

#endif
