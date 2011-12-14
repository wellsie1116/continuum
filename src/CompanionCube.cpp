
#include "CompanionCube.h"

void setTransform(const NewtonBody* body, const dFloat* matrix, int thread);
void applyForce(const NewtonBody* body, dFloat timestep, int thread);

CompanionCube::CompanionCube(Ogre::SceneNode* node, NewtonWorld* world)
	: mNode(node)
{
	NewtonCollision* shape = createBox(world);
	mBody = createRigidBody(world, shape);
	NewtonReleaseCollision(world, shape);
}

CompanionCube::~CompanionCube()
{
}

void CompanionCube::sync()
{

}

NewtonCollision*
CompanionCube::createBox(NewtonWorld* world)
{
	NewtonCollision* collision = NewtonCreateBox(world, 100.0f, 100.0f, 100.0f, 0, NULL);

	return collision;
}

NewtonBody*
CompanionCube::createRigidBody(NewtonWorld* world, NewtonCollision* shape)
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
	NewtonBodySetMassMatrix(body, 10, 1, 1, 1);

	NewtonBodySetUserData(body, this);

	NewtonBodySetForceAndTorqueCallback(body, CompanionCube::applyForceCallback);
	NewtonBodySetTransformCallback(body, CompanionCube::setTransformCallback);

	return body;
}

void
CompanionCube::setTransformCallback(const NewtonBody* body, const float* matrix, int thread)
{
	CompanionCube* self = (CompanionCube*)NewtonBodyGetUserData(body);
	self->setTransform(matrix, thread);
}

void
CompanionCube::applyForceCallback(const NewtonBody* body, dFloat timestep, int thread)
{ 
	CompanionCube* self = (CompanionCube*)NewtonBodyGetUserData(body);
	self->applyForce(timestep, thread);
}

void
CompanionCube::setTransform(const float* matrix, int thread)
{
	float quat[4];
	NewtonBodyGetRotation(mBody, quat);

	mNode->setPosition(matrix[12], matrix[13], matrix[14]);
	mNode->setOrientation(quat[0], quat[1], quat[2], quat[3]);
}

void
CompanionCube::applyForce(dFloat timestep, int thread)
{ 
	float gravity = -98.0f;
	float mass, ix, iy, iz;
	NewtonBodyGetMassMatrix(mBody, &mass, &ix, &iy, &iz);

	float force[4] = { 0.0f, gravity * mass, 0.0f, 1.0f };
	NewtonBodySetForce(mBody, force);
}

