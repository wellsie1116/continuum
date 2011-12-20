
#include "Box.h"

#include <OgreMovableObject.h>

#include "PhysicsWorld.h"

void setTransform(const NewtonBody* body, const dFloat* matrix, int thread);
void applyForce(const NewtonBody* body, dFloat timestep, int thread);

Box::Box(Ogre::SceneNode* node, NewtonWorld* world, float mass)
	: mNode(node)
	, mMass(mass)
{
	NewtonCollision* shape = createBox(world);
	mBody = createRigidBody(world, shape);
	NewtonReleaseCollision(world, shape);
}

Box::~Box()
{
}

void Box::sync()
{
	float matrix[16];
	float quat[4];

	NewtonBodyGetMatrix(mBody, matrix);
	NewtonBodyGetRotation(mBody, quat);

	mNode->setPosition(matrix[12], matrix[13], matrix[14]);
	mNode->setOrientation(quat[0], quat[1], quat[2], quat[3]);
}

NewtonCollision*
Box::createBox(NewtonWorld* world)
{
	Ogre::AxisAlignedBox bounds;
	bounds.setNull();

	Ogre::SceneNode::ObjectIterator it = mNode->getAttachedObjectIterator();
	while (it.hasMoreElements())
	{
		Ogre::MovableObject* obj = it.getNext();
		const Ogre::AxisAlignedBox& box = obj->getBoundingBox();
		bounds.merge(box);
	}
	Ogre::Vector3 range = (bounds.getMaximum() - bounds.getMinimum()) * mNode->getScale();
	Ogre::Vector3 min = Ogre::Vector3(0.1f);
	range.makeCeil(min);

	NewtonCollision* collision = NewtonCreateBox(world, range.x, range.y, range.z, 0, NULL);
	//printf("Range: %.1f, %.1f, %.1f\n", range.x, range.y, range.z);

	return collision;
}

NewtonBody*
Box::createRigidBody(NewtonWorld* world, NewtonCollision* shape)
{
	Ogre::Matrix4 transform;
	transform.makeTransform(
			mNode->getPosition(),
			Ogre::Vector3::UNIT_SCALE,
			mNode->getOrientation()
			);

	float matrix[16];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix[j*4+i] = transform[i][j];

	NewtonBody* body = NewtonCreateBody(world, shape, matrix);
	NewtonBodySetMassMatrix(body, mMass, 1, 1, 1);

	NewtonBodySetUserData(body, this);

	NewtonBodySetForceAndTorqueCallback(body, Box::applyForceCallback);
	NewtonBodySetTransformCallback(body, Box::setTransformCallback);

	//NewtonBodySetAutoSleep(body, 0);

	return body;
}

void
Box::setTransformCallback(const NewtonBody* body, const float* matrix, int thread)
{
	Box* self = (Box*)NewtonBodyGetUserData(body);
	self->setTransform(matrix, thread);
}

void
Box::applyForceCallback(const NewtonBody* body, dFloat timestep, int thread)
{ 
	Box* self = (Box*)NewtonBodyGetUserData(body);
	self->applyForce(timestep, thread);
}

void
Box::setTransform(const float* matrix, int thread)
{
	float quat[4];
	NewtonBodyGetRotation(mBody, quat);

	mNode->setPosition(matrix[12], matrix[13], matrix[14]);
	mNode->setOrientation(quat[0], quat[1], quat[2], quat[3]);
}

void
Box::applyForce(dFloat timestep, int thread)
{ 
	//if (getWorld()->getTimestep() == 1200)
	//{
	//	float force[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//	NewtonBodySetForce(mBody, force);
	//	return;
	//}

	float gravity = -9.8f;
	float mass, ix, iy, iz;
	NewtonBodyGetMassMatrix(mBody, &mass, &ix, &iy, &iz);

	float force[3] = {0.0f, gravity * mass, 0.0f};
	NewtonBodySetForce(mBody, force);
	
	float torque[3] = {0.0f, 0.0f, 0.0f};
	NewtonBodySetTorque(mBody, torque);
}
	
PhysicsWorld* 
Box::getWorld()
{
	return (PhysicsWorld*)NewtonWorldGetUserData(NewtonBodyGetWorld(mBody));
}


