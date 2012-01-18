
#include "PhysicsState.h"

#include "PhysicsWorld.h"

WorldSnapshot::WorldSnapshot(PhysicsWorld* world)
	: mWorld(world)
{
	mSeed = dRandGetSeed();

	dSpaceID space = world->getSpace();
	mBodyCount = dSpaceGetNumGeoms(space);
	mStates = new BodyState*[mBodyCount];

	for (int i = 0; i < mBodyCount; i++)
	{
		mStates[i] = new BodyState(dSpaceGetGeom(space, i));
	}
}

WorldSnapshot::~WorldSnapshot()
{
	if (mStates)
	{
		for (int i = 0; i < mBodyCount; i++)
		{
			delete mStates[i];
		}
		delete [] mStates;
		mStates = NULL;
	}
}

void
WorldSnapshot::restore()
{
	for (int i = 0; i < mBodyCount; i++)
	{
		PhysicsObject* obj = mStates[i]->getPhysObject();
		mStates[i]->restore();
	}

	dRandSetSeed(mSeed);
}

BodyState::BodyState(dGeomID geom)
	: mGeom(geom)
	, state(NULL)
{
	dGeomCopyPosition(mGeom, mPosition);
	dGeomCopyRotation(mGeom, mOrientation);

	mBody = dGeomGetBody(mGeom);
	if (mBody)
	{
		const float* vect;

		vect = dBodyGetLinearVel(mBody);
		for (int i = 0; i < 3; i++)
			mLinearVelocity[i] = vect[i];

		vect = dBodyGetAngularVel(mBody);
		for (int i = 0; i < 3; i++)
			mAngularVelocity[i] = vect[i];

		PhysicsObject* obj = (PhysicsObject*)dBodyGetData(mBody);
		if (obj)
		{
			state = obj->save();
			if (state)
			{
				state = state->copy();
			}
		}
		
		//vect = dBodyGetForce(mBody);
		//for (int i = 0; i < 3; i++)
		//	mForce[i] = vect[i];
		//
		//vect = dBodyGetTorque(mBody);
		//for (int i = 0; i < 3; i++)
		//	mTorque[i] = vect[i];
	}
}

BodyState::~BodyState()
{
	if (state)
	{
		delete state;
		state = NULL;
	}
}

void
BodyState::restore()
{
	dGeomSetPosition(mGeom, mPosition[0], mPosition[1], mPosition[2]);
	dGeomSetRotation(mGeom, mOrientation);

	if (mBody)
	{
		dBodySetLinearVel(mBody, mLinearVelocity[0], mLinearVelocity[1], mLinearVelocity[2]);
		dBodySetAngularVel(mBody, mAngularVelocity[0], mAngularVelocity[1], mAngularVelocity[2]);
		
		PhysicsObject* obj = (PhysicsObject*)dBodyGetData(mBody);
		if (obj)
		{
			obj->restore(state);
		}
		//dBodySetForce(mBody, mForce[0], mForce[1], mForce[2]);
		//dBodySetTorque(mBody, mTorque[0], mTorque[1], mTorque[2]);
	}
}

PhysicsObject*
BodyState::getPhysObject()
{
	return (PhysicsObject*)dGeomGetData(mGeom);
}

