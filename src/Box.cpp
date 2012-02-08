
#include "Box.h"

#include <OgreMovableObject.h>
#include <OgreEntity.h>
#include <OgreAnimationState.h>
#include <OgreSkeletonInstance.h>
#include <OgreBone.h>

#include "PhysicsWorld.h"

Box::Box(Ogre::SceneNode* node, PhysicsWorld* world, float mass)
	: mNode(node)
	, mBody(0)
	, mGeom(0)
	, mPos(NULL)
	, mMass(mass)
{
	Ogre::Vector3 offset;
	Ogre::Vector3 range = getBounds(mNode, offset);

	dMass m;
	dMassSetBox(&m, 1, range.x, range.y, range.z);
	dMassAdjust(&m, mass);

	Ogre::Vector3 pos = mNode->getPosition();
	const float* quat = mNode->getOrientation().ptr();
	mBody = dBodyCreate(world->getWorld());
	dBodySetPosition(mBody, pos.x + offset.x, pos.y + offset.y, pos.z + offset.z);
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
	Ogre::Vector3 offset;
	Ogre::Vector3 range = getBounds(mNode, offset);

	Ogre::Vector3 pos = mNode->getPosition();
	const float* quat = mNode->getOrientation().ptr();
	mGeom = dCreateBox(world->getSpace(), range.x, range.y, range.z);
	dGeomSetPosition(mGeom, pos.x + offset.x, pos.y + offset.y, pos.z + offset.z);
	dGeomSetQuaternion(mGeom, quat);
	dGeomSetData(mGeom, this);

	mPos = dGeomGetPosition(mGeom);
	
	//disable shadow casting
	Ogre::SceneNode::ObjectIterator it = node->getAttachedObjectIterator();
	while (it.hasMoreElements())
	{
		Ogre::MovableObject* obj = it.getNext();
		obj->setCastShadows(false);
	}
}

Surface::~Surface()
{
}

void
Surface::linkToggled()
{
	Ogre::SceneNode::ObjectIterator it = mNode->getAttachedObjectIterator();
	while (it.hasMoreElements())
	{
		Ogre::MovableObject* obj = it.getNext();
		Ogre::Entity* entity = dynamic_cast<Ogre::Entity*>(obj);
		if (!entity)
			continue;

		Ogre::AnimationStateSet* anims = entity->getAllAnimationStates();
		if (!anims->hasAnimationState("Activate"))
			continue;

		Ogre::AnimationState* anim = anims->getAnimationState("Activate");
		anim->setLoop(false);
		anim->setEnabled(true);
		//if (mLinkEnabled)
		//{
		//	anim->setTimePosition(0.0);
		//}
		//else
		//{
		//	anim->setTimePosition(anim->getLength());
		//}
	}
}

bool 
Surface::contains(dGeomID geom)
{
	return mGeom == geom;
}

void Surface::setupForces()
{
	bool update = false;
	Ogre::SceneNode::ObjectIterator it = mNode->getAttachedObjectIterator();
	while (it.hasMoreElements())
	{
		Ogre::MovableObject* obj = it.getNext();
		Ogre::Entity* entity = dynamic_cast<Ogre::Entity*>(obj);
		if (!entity)
			continue;

		Ogre::AnimationStateSet* animSet = entity->getAllAnimationStates();
		if (!animSet)
			continue;

		Ogre::AnimationStateIterator anims = animSet->getAnimationStateIterator();
		while (anims.hasMoreElements())
		{
			Ogre::AnimationState* anim = anims.getNext();
			if (!anim->getEnabled())
				continue;

			//FIXME save and restore anim times
			if (mLinkEnabled)
			{
				anim->addTime(1.0/TICKS_PER_SECOND);
			}
			else
			{
				anim->addTime(-1.0/TICKS_PER_SECOND);
			}
			update = true;
		}
	}

	if (!update)
		return;

	it = mNode->getAttachedObjectIterator();
	while (it.hasMoreElements())
	{
		Ogre::MovableObject* obj = it.getNext();
		Ogre::Entity* entity = dynamic_cast<Ogre::Entity*>(obj);
		if (!entity || !entity->hasSkeleton())
			continue;

		Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
		Ogre::Bone* bone = skeleton->getRootBone();
		Ogre::Vector3 anim = bone->getPosition();

		Ogre::Vector3 offset;
		Ogre::Vector3 range = getBounds(mNode, offset);
		Ogre::Vector3 pos = mNode->getPosition();
		dGeomSetPosition(mGeom,
				pos.x + offset.x + anim.x, 
				pos.y + offset.y + anim.y, 
				pos.z + offset.z + anim.z
				);
	}
}

void Surface::sync()
{
	//dQuaternion quat;
	//dGeomGetQuaternion(mGeom, quat);

	//mNode->setPosition(mPos[0], mPos[1], mPos[2]);
	//mNode->setOrientation(quat[0], quat[1], quat[2], quat[3]);
}

void
physics_object_free(PhysicsObject* object)
{
	delete object;
}

