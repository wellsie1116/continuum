
#ifndef PHYSICSSTATE_H_FG1UCOI5
#define PHYSICSSTATE_H_FG1UCOI5

#include "WorldObject.h"
#include "Box.h"

#include <ode/ode.h>
#include <glib.h>

class PhysicsWorld;

class BodyState
{
public:
	BodyState(dGeomID geom);
	~BodyState();

	void restore();
	PhysicsObject* getPhysObject();

private:
	dGeomID mGeom;
	dBodyID mBody;

	dVector3 mPosition;
	dMatrix3 mOrientation;
	float mLinearVelocity[3];
	float mAngularVelocity[3];
	const PhysicsObjectState* state;
	//float mForce[3];
	//float mTorque[3];
};

class WorldSnapshot : public WorldObjectState
{
public:
	WorldSnapshot(PhysicsWorld* world);
	virtual ~WorldSnapshot();

	void restore();

private:
	PhysicsWorld* mWorld;
	int mTimestep;

	BodyState** mStates;
	int mBodyCount;
	unsigned long mSeed;
};

#endif

