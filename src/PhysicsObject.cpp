
#include "PhysicsObject.h"

#include <OgreMovableObject.h>

Ogre::Vector3 PhysicsObject::getBounds(Ogre::SceneNode* node, Ogre::Vector3& offset)
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

	//{
	//	Ogre::Vector3 min = bounds.getMinimum();
	//	Ogre::Vector3 max = bounds.getMaximum();
	//	printf("Bounds: (%.1f, %.1f, %.1f) (%.1f, %.1f, %.1f)\n",
	//			min[0], min[1], min[2],
	//			max[0], max[1], max[2]
	//			);
	//}

	offset = bounds.getCenter();

	return range;
}

