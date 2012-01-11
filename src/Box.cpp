
#include "Box.h"

#include <OgreMovableObject.h>

#include "PhysicsWorld.h"

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

	mPos = dGeomGetPosition(mGeom);
}

Surface::~Surface()
{
}

void Surface::sync()
{
	dQuaternion quat;
	dGeomGetQuaternion(mGeom, quat);

	mNode->setPosition(mPos[0], mPos[1], mPos[2]);
	mNode->setOrientation(quat[0], quat[1], quat[2], quat[3]);
}

void
physics_object_free(PhysicsObject* object)
{
	delete object;
}

