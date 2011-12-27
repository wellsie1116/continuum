
#ifndef PHYSICSSTATE_H_FG1UCOI5
#define PHYSICSSTATE_H_FG1UCOI5

#include "Box.h"

#include <ode/ode.h>
#include <glib.h>

class PhysicsWorld;

class BodyState
{
public:
	BodyState(dGeomID geom);

	void restore();
	PhysicsObject* getPhysObject();

private:
	dGeomID mGeom;
	dBodyID mBody;

	dVector3 mPosition;
	dMatrix3 mOrientation;
	float mLinearVelocity[3];
	float mAngularVelocity[3];
	//float mForce[3];
	//float mTorque[3];
};

class WorldSnapshot
{
public:
	WorldSnapshot(PhysicsWorld* world, int timestep);
	~WorldSnapshot();

	void restore();
	int getTimestep() { return mTimestep; }

private:
	PhysicsWorld* mWorld;
	int mTimestep;

	BodyState** mStates;
	int mBodyCount;
	unsigned long mSeed;
};

//class SnapshotSet
//{
//public:
//	void add(int timestep, WorldSnapshot* state); //tree
//	bool contains(int timestep);   //tree
//	void purgeAfter(int timestep); //array/list
//	int getClosest(int timestep);  //array
//	void prune(int timestep);      //list
//};

class SnapshotManager
{
public:
	SnapshotManager(PhysicsWorld* world);
	~SnapshotManager();

	void reset();
	void worldTick(int timestep);
	void restoreSnapshot(int timestep);

private:
	PhysicsWorld* mWorld;
	GQueue* mStates;
	WorldSnapshot* mStartState;
};

#endif

