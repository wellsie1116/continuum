
#ifndef PLAYER_H_T7NTQBP4
#define PLAYER_H_T7NTQBP4

#include <OgreSceneNode.h>
#include <OgreCamera.h>

#include <ode/ode.h>

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

public:
	PlayerState()
		: moveDirection(NONE)
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

	void startMove(PlayerDirection dir);
	void stopMove(PlayerDirection dir);

protected:
	Ogre::Camera* mCamera;
	dBodyID mPlayerBody;
	dGeomID mPlayerBodyGeom;
	const float* mPos;
	PlayerState state;
};

#endif
