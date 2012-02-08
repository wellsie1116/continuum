
#ifndef WORLDOBJECT_H_DAGPXJA6
#define WORLDOBJECT_H_DAGPXJA6

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

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

class InputController
{
public:
	virtual void injectKeyDown(const OIS::KeyEvent &arg) = 0;
	virtual void injectKeyUp(const OIS::KeyEvent &arg) = 0;
	virtual void injectMouseMove(const OIS::MouseEvent &arg) = 0;
	virtual void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
	virtual void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
};

class InputControllerDup
{
public:
	virtual InputController* duplicate(InputController* obj) = 0;
};

#endif
