
#include "Player.h"

#include <OgreMovableObject.h>

#include "PhysicsWorld.h"

#define CAMERA_RADIUS 0.9
#define CAMERA_MASS 0.1

#define PITCH_MAX 3600

#define PLAYER_MASS 30.0
#define PLAYER_HEIGHT 5.0
#define PLAYER_CAMERA_HEIGHT 4.0
#define PLAYER_CAMERA_RADIUS 20.0
#define PLAYER_RADIUS 3.0

Player::Player(Ogre::Camera* camera, PhysicsWorld* world)
	: mCamera(camera)
	, xDiff(0)
	, mPlayerNode(NULL)
	, mCameraTrackNode(NULL)
{
	Ogre::Vector3 cameraPos = camera->getPosition();
	const float* cameraQuat = camera->getOrientation().ptr();

	{
		dMass mass;
		dMassSetCapsuleTotal(&mass, PLAYER_MASS, 2, PLAYER_RADIUS, PLAYER_HEIGHT);
		mPlayerBody = dBodyCreate(world->getWorld());
		dBodySetPosition(mPlayerBody, 
				cameraPos.x, 
				cameraPos.y + PLAYER_HEIGHT/2.0, 
				cameraPos.z);
		dBodySetQuaternion(mPlayerBody, cameraQuat);
		dBodySetMaxAngularSpeed(mPlayerBody, 0);
		//dBodySetLinearDamping(mPlayerBody, 0.001);
		dBodySetMass(mPlayerBody, &mass);
	}

	{
		dMass mass;
		dMassSetSphereTotal(&mass, CAMERA_MASS, CAMERA_RADIUS);

		Ogre::Vector3 z = camera->getOrientation().zAxis();
		Ogre::Vector3 pos(z[0], 0.0, z[2]);
		pos.normalise();
		pos *= PLAYER_CAMERA_RADIUS;
		pos[1] += PLAYER_HEIGHT + PLAYER_CAMERA_HEIGHT;
		pos += cameraPos;

		mCameraBody = dBodyCreate(world->getWorld());
		dBodySetPosition(mCameraBody, pos.x, pos.y, pos.z);
		dBodySetQuaternion(mCameraBody, cameraQuat);
		dBodySetMass(mCameraBody, &mass);
	}

	{
		mPlayerBodyGeom = dCreateCapsule(world->getSpace(), PLAYER_RADIUS, PLAYER_HEIGHT);
		dGeomSetBody(mPlayerBodyGeom, mPlayerBody);
		dGeomSetData(mPlayerBodyGeom, this);
		dQuaternion geomQuat;
		dQFromAxisAndAngle(geomQuat, 1.0, 0.0, 0.0, M_PI/2.0);
		dGeomSetOffsetPosition(mPlayerBodyGeom, 0.0, -PLAYER_CAMERA_HEIGHT/2.0, 0.0);
		dGeomSetOffsetQuaternion(mPlayerBodyGeom, geomQuat);
	}
	
	{
		mCameraBodyGeom = dCreateSphere(world->getSpace(), CAMERA_RADIUS);
		dGeomSetBody(mCameraBodyGeom, mCameraBody);
		dGeomSetData(mCameraBodyGeom, this);
	}

	{
		mCameraSwivel = dJointCreateHinge(world->getWorld(), 0);
		dJointAttach(mCameraSwivel, mPlayerBody, mCameraBody);
		dJointSetHingeAnchor(mCameraSwivel, 
				cameraPos.x, 
				cameraPos.y + PLAYER_HEIGHT, 
				cameraPos.z);
		dJointSetHingeAxis(mCameraSwivel, 0.0, 1.0, 0.0);
		//dJointSetHinge2Axis2(mCameraSwivel, 1.0, 0.0, 0.0);
	}
	
	{
		//Ogre::Entity* ogreHead = mSceneMgr->createEntity( "Player", "ogrehead.mesh" );
		//mCameraNode = mCamera->getRootSceneNode()->createChildSceneNode( "HeadNode2", Ogre::Vector3( 0, 0, 0 ) );
		//headNode2->attachObject( ogreHead2 );
	}

	mPlayerPos = dBodyGetPosition(mPlayerBody);
	mCameraPos = dBodyGetPosition(mCameraBody);
}

Player::~Player()
{
}

#define MOVE_FORCE PLAYER_MASS * 10.0
#define SWIVEL_TORQUE 20.0

#define JUMP_STEP_MAX 10
	
void
Player::setupForces()
{
	const Ogre::Quaternion& quat = mPlayerNode->getOrientation();
	Ogre::Vector3 x = quat.xAxis();
	Ogre::Vector3 z = quat.zAxis();
	x *= MOVE_FORCE;
	z *= MOVE_FORCE;

	if (state.moveDirection & FORWARD)
		dBodyAddRelForce(mPlayerBody, z[0], z[1], z[2]);
	if (state.moveDirection & BACKWARD)
		dBodyAddRelForce(mPlayerBody, -z[0], -z[1], -z[2]);
	if (state.moveDirection & LEFT)
		dBodyAddRelForce(mPlayerBody, x[0], x[1], x[2]);
	if (state.moveDirection & RIGHT)
		dBodyAddRelForce(mPlayerBody, -x[0], -x[1], -x[2]);

	if (state.moveDirection & JUMP)
	{
		if (state.jumpStep < JUMP_STEP_MAX)
		{
			dBodyAddRelForce(mPlayerBody, 0.0, 100.0 * PLAYER_MASS, 0.0);
			state.jumpStep++;
		}
	}
	else if (state.jumpStep)
	{
		state.jumpStep = 0;
	}

	dBodyAddTorque(mCameraBody, 0.0, -xDiff * SWIVEL_TORQUE, 0.0);

	//TODO dampen player velocity
	//dReal* playerVelocity = dBodyGetLinearVel(mPlayerBody);
	//if (!(state.moveDirection & (FORWARD | BACKWARD)))
	//{
	//}
	//if (!(state.moveDirection & (LEFT | RIGHT)))
	//{
	//}
	
	if (!xDiff)
	{
		const float* vel = dBodyGetLinearVel(mCameraBody);
		dBodyAddForce(mCameraBody,
				-vel[0] * CAMERA_MASS * 10, 
				-vel[1] * CAMERA_MASS * 10, 
				-vel[2] * CAMERA_MASS * 10);
	}

	xDiff = 0;
}

void Player::sync()
{
	mCamera->setPosition(mCameraPos[0], mCameraPos[1], mCameraPos[2]);

	if (mPlayerNode)
	{
		mPlayerNode->setPosition(
				mPlayerPos[0],
				mPlayerPos[1] - PLAYER_HEIGHT/2.0,
				mPlayerPos[2]);

		Ogre::Vector3 orientation(
				mPlayerPos[0] - mCameraPos[0],
				0.0,
				mPlayerPos[2] - mCameraPos[2]);
		Ogre::Quaternion swivel = Ogre::Vector3::UNIT_Z.getRotationTo(orientation);
		mPlayerNode->setOrientation(swivel);
	}
	
	if (mCameraTrackNode)
	{
		mCameraTrackNode->setPosition(
				Ogre::Vector3::UNIT_Y * (PLAYER_HEIGHT/2.0 +
				2 * PLAYER_HEIGHT * state.pitch / PITCH_MAX));
	}

	//Ogre::Vector3 orientation(
	//		mPlayerPos[0] - mCameraPos[0],
	//		0.0,
	//		mPlayerPos[2] - mCameraPos[2]);
	//orientation.normalise();

	//Ogre::Quaternion quat(Ogre::Radian(state.pitch * M_PI * 0.9 / PITCH_MAX),
	//	   Ogre::Vector3::UNIT_X);
	//Ogre::Quaternion swivel = Ogre::Vector3::UNIT_X.getRotationTo(orientation);
	//quat = swivel * quat;

	//Ogre::Quaternion quat(mRot[0], mRot[1], mRot[2], mRot[3]);
	//mCamera->setOrientation(quat);
	
}
		
const PhysicsObjectState* Player::save() const
{
	return &state;
}

void Player::restore(const PhysicsObjectState* state)
{
	this->state = *(PlayerState*)state;
}

void Player::setNode(Ogre::SceneNode* node)
{
	mPlayerNode = node;

	if (mPlayerNode)
	{
		mCameraTrackNode = mPlayerNode->createChildSceneNode(
				Ogre::Vector3::UNIT_Y * PLAYER_HEIGHT/2.0);
		mCamera->setAutoTracking(true, mCameraTrackNode);
	}
}

void
Player::injectKeyDown(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{
		case OIS::KC_W:
			startMove(FORWARD);
			break;
		case OIS::KC_S:
			startMove(BACKWARD);
			break;
		case OIS::KC_A:
			startMove(LEFT);
			break;
		case OIS::KC_D:
			startMove(RIGHT);
			break;
		case OIS::KC_SPACE:
			startMove(JUMP);
			break;
    }
}

void
Player::injectKeyUp(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{
		case OIS::KC_W:
			stopMove(FORWARD);
			break;
		case OIS::KC_S:
			stopMove(BACKWARD);
			break;
		case OIS::KC_A:
			stopMove(LEFT);
			break;
		case OIS::KC_D:
			stopMove(RIGHT);
			break;
		case OIS::KC_SPACE:
			stopMove(JUMP);
			break;
    }
}
	
void Player::injectMouseMove(const OIS::MouseEvent &arg)
{
	int x = arg.state.X.rel;
	int y = arg.state.Y.rel;

	xDiff += x;

	state.pitch -= y;
	if (state.pitch > PITCH_MAX)
		state.pitch = PITCH_MAX;
	else if (state.pitch  < -PITCH_MAX)
		state.pitch = -PITCH_MAX;
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

