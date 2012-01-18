
#ifndef WORLD_H_JP8SN8HW
#define WORLD_H_JP8SN8HW

#include "WorldObject.h"
#include "TickTimer.h"

#include <glib.h>

#define TICKS_PER_SECOND 120.0

class World
{
public:
	World();
	~World();

public:
	void addObject(WorldObject* object);

	void start();

	void freezeTime();
	void resumeTime();
	void accelerateTime();
	void decelerateTime();
	
	void step();

private:
	void updateTimeRate();

private:
	TickTimer mTimer;
	int mStepRate;
	unsigned long mTimestep;

	GQueue* mObjects; //WorldObject
};

#endif
