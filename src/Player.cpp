
#include "Player.h"

#include <OgreMovableObject.h>

#include "PhysicsWorld.h"

#define PLAYER_HEIGHT 100.0
#define PLAYER_RADIUS 1.0

Player::Player(Ogre::Camera* camera, PhysicsWorld* world)
	: mCamera(camera)
{
	double radius = 1.0;
	dMass mass;
	dMassSetCapsuleTotal(&mass, 10.0, 2, PLAYER_RADIUS, PLAYER_HEIGHT);
	Ogre::Vector3 pos = camera->getPosition();
	const float* quat = camera->getOrientation().ptr();
	mPlayerBody = dBodyCreate(world->getWorld());
	dBodySetPosition(mPlayerBody, pos.x, pos.y + PLAYER_HEIGHT/2.0, pos.z);
	dBodySetQuaternion(mPlayerBody, quat);
	dBodySetMaxAngularSpeed(mPlayerBody, 0);
	dBodySetMass(mPlayerBody, &mass);

	mPlayerBodyGeom = dCreateCapsule(world->getSpace(), PLAYER_RADIUS, PLAYER_HEIGHT);
	//dGeomSetPosition(mPlayerBodyGeom, pos.x, pos.y, pos.z);
	//dGeomSetPosition(mPlayerBodyGeom, 0.0, PLAYER_HEIGHT/2, 0.0);
	dGeomSetBody(mPlayerBodyGeom, mPlayerBody);
	dGeomSetData(mPlayerBodyGeom, this);
	//dGeomSetOffsetPosition(mPlayerBodyGeom, 0.0, PLAYER_HEIGHT/2, 0.0);
	dQuaternion geomQuat;
	dQFromAxisAndAngle(geomQuat, 1.0, 0.0, 0.0, M_PI/2.0);
	dGeomSetOffsetQuaternion(mPlayerBodyGeom, geomQuat);

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

#define MOVE_FORCE 100.0
	
void
Player::setupForces()
{
	if (state.moveDirection & FORWARD)
		dBodyAddRelForce(mPlayerBody, 0.0, 0.0, -MOVE_FORCE);
	if (state.moveDirection & BACKWARD)
		dBodyAddRelForce(mPlayerBody, 0.0, 0.0, MOVE_FORCE);
	if (state.moveDirection & LEFT)
		dBodyAddRelForce(mPlayerBody, -MOVE_FORCE, 0.0, 0.0);
	if (state.moveDirection & RIGHT)
		dBodyAddRelForce(mPlayerBody, MOVE_FORCE, 0.0, 0.0);
}

void Player::sync()
{
	mCamera->setPosition(mPos[0], mPos[1], mPos[2]);
	//TODO orientation
}
		
const PhysicsObjectState* Player::save() const
{
	return &state;
}

void Player::restore(const PhysicsObjectState* state)
{
	this->state = *(PlayerState*)state;
}

void
Player::startMove(PlayerDirection dir)
{
	state.moveDirection = (PlayerDirection)(state.moveDirection | dir);
}

void
Player::stopMove(PlayerDirection dir)
{
	state.moveDirection = (PlayerDirection)(state.moveDirection & ~dir);
}

