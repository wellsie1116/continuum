
#ifndef PLAYER_H_T7NTQBP4
#define PLAYER_H_T7NTQBP4

#include <OgreSceneNode.h>
#include <OgreCamera.h>

#include <ode/ode.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "PhysicsObject.h"

class PhysicsWorld;

enum PlayerDirection {
	NONE     = 0,
	FORWARD  = 1 << 0,
	BACKWARD = 1 << 1,
	LEFT     = 1 << 2,
	RIGHT    = 1 << 3,
	JUMP     = 1 << 4
};

class PlayerState : public PhysicsObjectState
{
public:
	PlayerDirection moveDirection;
	int pitch;

public:
	PlayerState()
		: moveDirection(NONE)
		, pitch(0)
	{ }

	PhysicsObjectState* copy() const
	{
		PlayerState* res = new PlayerState();
		*res = *this;
		return res;
	}

	PlayerState& operator=(const PlayerState& state)
	{
		moveDirection = state.moveDirection;
		pitch = state.pitch;
		return *this;
	}
};

class Player : public PhysicsObject
{
public:
	Player(Ogre::Camera* camera, PhysicsWorld* world);
	virtual ~Player();
	
	virtual void setupForces();
	virtual void sync();
	virtual const PhysicsObjectState* save() const;
	virtual void restore(const PhysicsObjectState* state);

	Ogre::Camera* getCamera() { return mCamera; }

	void setNode(Ogre::SceneNode* node);

	void injectKeyDown(const OIS::KeyEvent &arg);
	void injectKeyUp(const OIS::KeyEvent &arg);
	void injectMouseMove(const OIS::MouseEvent &arg);

private:
	void startMove(PlayerDirection dir);
	void stopMove(PlayerDirection dir);

protected:
	Ogre::Camera* mCamera;
	dBodyID mPlayerBody;
	dGeomID mPlayerBodyGeom;
	dBodyID mCameraBody;
	dGeomID mCameraBodyGeom;
	dJointID mCameraSwivel;
	const float* mPlayerPos;
	const float* mCameraPos;
	PlayerState state;

	Ogre::SceneNode* mPlayerNode;
	Ogre::SceneNode* mCameraTrackNode;
	//Ogre::SceneNode* mCameraNode;

	int xDiff;
};

#endif
