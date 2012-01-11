
#include "Player.h"

#include <OgreMovableObject.h>

#include "PhysicsWorld.h"

#define PLAYER_HEIGHT 10.0
#define PLAYER_RADIUS 1.0

Player::Player(Ogre::Camera* camera, PhysicsWorld* world)
	: mCamera(camera)
	, moveDirection(NONE)
{
	double radius = 1.0;
	dMass mass;
	dMassSetCapsuleTotal(&mass, 1.0, 1, PLAYER_RADIUS, PLAYER_HEIGHT);
	Ogre::Vector3 pos = camera->getPosition();
	const float* quat = camera->getOrientation().ptr();
	mPlayerBody = dBodyCreate(world->getWorld());
	dBodySetPosition(mPlayerBody, pos.x, pos.y, pos.z);
	dBodySetQuaternion(mPlayerBody, quat);
	dBodySetMaxAngularSpeed(mPlayerBody, 0);
	dBodySetMass(mPlayerBody, &mass);

	mPlayerBodyGeom = dCreateCapsule(world->getSpace(), PLAYER_RADIUS, PLAYER_HEIGHT);
	dGeomSetBody(mPlayerBodyGeom, mPlayerBody);
	dGeomSetData(mPlayerBodyGeom, this);
	dGeomSetOffsetPosition(mPlayerBodyGeom, 0.0, PLAYER_HEIGHT/2, 0.0);

	mPos = dBodyGetPosition(mPlayerBody);


	//Ogre::Vector3 range = getBounds(mNode);

	//dMass m;
	//dMassSetBox(&m, 1, range.x, range.y, range.z);
	//dMassAdjust(&m, mass);

	//Ogre::Vector3 pos = mNode->getPosition();
	//const float* quat = mNode->getOrientation().ptr();
	//mBody = dBodyCreate(world->getWorld());
	//dBodySetPosition(mBody, pos.x, pos.y, pos.z);
	//dBodySetQuaternion(mBody, quat);
	//dBodySetMass(mBody, &m);

	//mGeom = dCreateBox(world->getSpace(), range.x, range.y, range.z);
	//dGeomSetBody(mGeom, mBody);
	//dGeomSetData(mGeom, this);

	//mPos = dBodyGetPosition(mBody);
}

Player::~Player()
{
}

void Player::sync()
{
	mCamera->setPosition(mPos[0], mPos[1], mPos[2]);
	//TODO orientation
}

void
Player::startMove(PlayerDirection dir)
{
	moveDirection = (PlayerDirection)(moveDirection | dir);
}

void
Player::stopMove(PlayerDirection dir)
{
	moveDirection = (PlayerDirection)(moveDirection & ~dir);
}

