
#include "Player.h"

#include <OgreMovableObject.h>

#include "PhysicsWorld.h"

#include "vect.h"

#define CAMERA_RADIUS 0.2
#define CAMERA_MASS 0.1

#define PITCH_MAX 3600

#define PLAYER_MASS 5.0
#define PLAYER_RADIUS 0.5
#define PLAYER_HEIGHT (1.8*2)
#define PLAYER_HEIGHT_OFFSET .1
#define PLAYER_CAPSULE_HEIGHT (PLAYER_HEIGHT-2*PLAYER_RADIUS)
#define PLAYER_CAMERA_HEIGHT 0.2
#define PLAYER_CAMERA_RADIUS 2.5

#define MOVE_FORCE 20.0
#define SWIVEL_TORQUE 0.2

#define JUMP_STEP_MAX 10
#define JUMP_FORCE 50.0

Player::Player(Ogre::Camera* camera, PhysicsWorld* world)
	: mCamera(camera)
	, xDiff(0)
	, mPlayerNode(NULL)
	, mCameraTrackNode(NULL)
{
	Ogre::Vector3 cameraPos = camera->getPosition();
	const float* cameraQuat = camera->getOrientation().ptr();

	g_assert((PLAYER_RADIUS + CAMERA_RADIUS) < PLAYER_CAMERA_RADIUS);

	{
		dMass mass;
		dMassSetCapsuleTotal(&mass, PLAYER_MASS, 2, PLAYER_RADIUS, PLAYER_CAPSULE_HEIGHT);
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
		mPlayerBodyGeom = dCreateCapsule(world->getSpace(), PLAYER_RADIUS, PLAYER_CAPSULE_HEIGHT);
		dGeomSetBody(mPlayerBodyGeom, mPlayerBody);
		dGeomSetData(mPlayerBodyGeom, this);
		dQuaternion geomQuat;
		dQFromAxisAndAngle(geomQuat, 1.0, 0.0, 0.0, M_PI/2.0);
		dGeomSetOffsetPosition(mPlayerBodyGeom, 0.0, PLAYER_CAMERA_HEIGHT, 0.0);
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
	
void
Player::setupForces()
{
	const Ogre::Quaternion& quat = mPlayerNode->getOrientation();
	Ogre::Vector3 x = quat.xAxis();
	Ogre::Vector3 z = quat.zAxis();

	Ogre::Vector3 dPlayer;
	Ogre::Vector3 dCamera;
	if (state.moveDirection & (FORWARD | BACKWARD | LEFT | RIGHT))
	{
		int dir = state.moveDirection & (FORWARD | BACKWARD | LEFT | RIGHT);
		if ((dir & (FORWARD | BACKWARD)) == (FORWARD | BACKWARD))
			dir &= ~(FORWARD | BACKWARD);
		if ((dir & (LEFT | RIGHT)) == (LEFT | RIGHT))
			dir &= ~(LEFT | RIGHT);
		switch (dir)
		{
			case FORWARD:
				dPlayer = z;
				dCamera = Ogre::Vector3(0.0);
				break;
			case LEFT:
				dPlayer = x;
				dCamera = x;
				break;
			case BACKWARD:
				dPlayer = -z;
				dCamera = Ogre::Vector3(0.0);
				break;
			case RIGHT:
				dPlayer = -x;
				dCamera = -x;
				break;
			case FORWARD | LEFT:
				dPlayer = z + x;
				dCamera = x;
				break;
			case FORWARD | RIGHT:
				dPlayer = z - x;
				dCamera = -x;
				break;
			case BACKWARD | LEFT:
				dPlayer = -z + x;
				dCamera = x;
				break;
			case BACKWARD | RIGHT:
				dPlayer = -z - x;
				dCamera = -x;
				break;
			default:
				g_assert_not_reached();
		}

		dPlayer.normalise();
		dPlayer *= MOVE_FORCE * PLAYER_MASS;
		dBodyAddForce(mPlayerBody, dPlayer[0], dPlayer[1], dPlayer[2]);

		if (!dCamera.isZeroLength())
		{
			dCamera.normalise();
			dCamera *= MOVE_FORCE * CAMERA_MASS;
			dBodyAddRelForce(mCameraBody, dCamera[0], dCamera[1], dCamera[2]);
		}
	}

	if (state.moveDirection & JUMP)
	{
		if (state.jumpStep < JUMP_STEP_MAX)
		{
			dBodyAddRelForce(mPlayerBody, 0.0, JUMP_FORCE*PLAYER_MASS, 0.0);
			dBodyAddRelForce(mCameraBody, 0.0, JUMP_FORCE*CAMERA_MASS, 0.0);
			state.jumpStep++;
		}
	}
	else if (state.jumpStep)
	{
		state.jumpStep = 0;
	}

	dBodyAddTorque(mCameraBody, 0.0, -xDiff * SWIVEL_TORQUE, 0.0);

	//dampen player velocity
	{
		const float* vel = dBodyGetLinearVel(mPlayerBody);
		if (state.moveDirection & (FORWARD | BACKWARD | LEFT | RIGHT))
		{
			Ogre::Vector3 move(dPlayer);
			move.normalise();

			Vector3 velocity = {vel[0], vel[1], vel[2]};
			Vector3 forward = {move[0], move[1], move[2]};
			Vector3 res;
			vect_project(velocity, forward, &res);
			vect_subtract(velocity, res, &res);

			dBodyAddForce(mPlayerBody, 
					-res.x * PLAYER_MASS * 5, 
					0.0, 
					-res.z * PLAYER_MASS * 5);
		}
		else
		{
			dBodyAddForce(mPlayerBody, 
					-vel[0] * PLAYER_MASS * 5, 
					0.0, 
					-vel[2] * PLAYER_MASS * 5);
		}
	}
	
	if (!xDiff)
	{
		const float* vel = dBodyGetLinearVel(mCameraBody);
		dBodyAddForce(mCameraBody,
				-vel[0] * CAMERA_MASS * 10, 
				0.0, 
				-vel[2] * CAMERA_MASS * 10);
	}

	xDiff = 0;
}

void Player::sync()
{
	mCamera->setPosition(
			mCameraPos[0],
			mCameraPos[1] - (0*PLAYER_HEIGHT/2.0 +
				4 * PLAYER_HEIGHT * state.pitch / PITCH_MAX), 
			mCameraPos[2]);

	if (mPlayerNode)
	{
		mPlayerNode->setPosition(
				mPlayerPos[0],
				mPlayerPos[1] - PLAYER_HEIGHT/2.0 + PLAYER_HEIGHT_OFFSET,
				mPlayerPos[2]);

		Ogre::Vector3 orientation(
				mPlayerPos[0] - mCameraPos[0],
				0.0,
				mPlayerPos[2] - mCameraPos[2]);
		Ogre::Quaternion swivel = Ogre::Vector3::UNIT_Z.getRotationTo(orientation);
		mPlayerNode->setOrientation(swivel);
	}
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

