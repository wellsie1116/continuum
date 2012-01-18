
#ifndef WORLDOBJECT_H_DAGPXJA6
#define WORLDOBJECT_H_DAGPXJA6

class WorldObjectState
{
public:
	virtual ~WorldObjectState() {}
};

class WorldObject
{
public:
	virtual void step() = 0;
	virtual void sync() = 0;
	virtual WorldObjectState* save() = 0;
	virtual void restore(WorldObjectState* state) = 0;
};

#endif
