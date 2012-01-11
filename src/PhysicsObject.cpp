
#include "PhysicsObject.h"

#include <OgreMovableObject.h>

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

