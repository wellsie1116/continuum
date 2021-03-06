
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
	int jumpStep;
	int pitch;

	int xDiff;

public:
	PlayerState()
		: moveDirection(NONE)
		, pitch(0)
		, jumpStep(0)
		, xDiff(0)
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
		jumpStep = state.jumpStep;
		xDiff = state.xDiff;
		return *this;
	}
};

class Player : public PhysicsObject
{
public:
	Player(Ogre::Camera* camera, PhysicsWorld* world);
	virtual ~Player();
	
	void remove();
	virtual void setupForces();
	virtual void sync();
	virtual const PhysicsObjectState* save() const;
	virtual void restore(const PhysicsObjectState* state);
	virtual bool contains(dGeomID geom);

	Ogre::Camera* getCamera() { return mCamera; }

	void setNode(Ogre::SceneNode* node);

	void injectKeyDown(const OIS::KeyEvent &arg);
	void injectKeyUp(const OIS::KeyEvent &arg);
	void injectMouseMove(const OIS::MouseEvent &arg);
	
	double getForwardVelocity();

	void initializeCamera(Ogre::Camera* camera);

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
};

#endif
