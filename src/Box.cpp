
#include "Box.h"

#include <OgreMovableObject.h>

#include "PhysicsWorld.h"

Ogre::Vector3 PhysicsObject::getBounds(Ogre::SceneNode* node)
{
	Ogre::AxisAlignedBox bounds;
	bounds.setNull();

	Ogre::SceneNode::ObjectIterator it = node->getAttachedObjectIterator();
	while (it.hasMoreElements())
	{
		Ogre::MovableObject* obj = it.getNext();
		const Ogre::AxisAlignedBox& box = obj->getBoundingBox();
		bounds.merge(box);
	}
	Ogre::Vector3 range = (bounds.getMaximum() - bounds.getMinimum()) * node->getScale();
	Ogre::Vector3 min = Ogre::Vector3(0.1f);
	range.makeCeil(min);

	return range;
}

Box::Box(Ogre::SceneNode* node, PhysicsWorld* world, float mass)
	: mNode(node)
	, mBody(0)
	, mGeom(0)
	, mPos(NULL)
	, mMass(mass)
{
	Ogre::Vector3 range = getBounds(mNode);

	dMass m;
	dMassSetBox(&m, 1, range.x, range.y, range.z);
	dMassAdjust(&m, mass);

	Ogre::Vector3 pos = mNode->getPosition();
	const float* quat = mNode->getOrientation().ptr();
	mBody = dBodyCreate(world->getWorld());
	dBodySetPosition(mBody, pos.x, pos.y, pos.z);
	dBodySetQuaternion(mBody, quat);
	dBodySetMass(mBody, &m);

	mGeom = dCreateBox(world->getSpace(), range.x, range.y, range.z);
	dGeomSetBody(mGeom, mBody);
	dGeomSetData(mGeom, this);

	mPos = dBodyGetPosition(mBody);
}

Box::~Box()
{
}

void Box::sync()
{
	const float* quat = dBodyGetQuaternion(mBody);

	mNode->setPosition(mPos[0], mPos[1], mPos[2]);
	mNode->setOrientation(quat[0], quat[1], quat[2], quat[3]);
}

Surface::Surface(Ogre::SceneNode* node, PhysicsWorld* world)
	: mNode(node)
	, mGeom(0)
{
	Ogre::Vector3 range = getBounds(mNode);

	Ogre::Vector3 pos = mNode->getPosition();
	const float* quat = mNode->getOrientation().ptr();
	mGeom = dCreateBox(world->getSpace(), range.x, range.y, range.z);
	dGeomSetPosition(mGeom, pos.x, pos.y, pos.z);
	dGeomSetQuaternion(mGeom, quat);
	dGeomSetData(mGeom, this);
}

Surface::~Surface()
{
}

void Surface::sync()
{
	//TODO implement
}
