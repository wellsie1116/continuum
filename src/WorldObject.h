
#ifndef WORLDOBJECT_H_DAGPXJA6
#define WORLDOBJECT_H_DAGPXJA6

class WorldObjectState
{
public:
	virtual WorldObjectState* copy() const = 0;
};

class WorldObject
{
public:
	virtual void step() = 0;
	virtual void sync() = 0;
	virtual const WorldObjectState* save() const = 0;
	virtual void restore(const WorldObjectState* state) = 0;
};

#endif
