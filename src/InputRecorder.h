
#ifndef INPUTRECORDER_H_8UQQLNC4
#define INPUTRECORDER_H_8UQQLNC4

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <glib.h>

#include "WorldObject.h"

struct Axis
{
	int abs, rel;
	bool absOnly;
	
	void init(OIS::Axis* axis);
};

struct MouseState
{
	int width, height;
	Axis X, Y, Z;
	int buttons;

	void init(OIS::MouseState* state);
};

struct InputEvent
{
	OIS::Object* device;
	union {
		struct {
			OIS::KeyCode key;
			unsigned int text;
		} key;
		struct {
			MouseState state;
			OIS::MouseButtonID id;
		} mouse;
	} event;
	enum {
		EVENT_KEY_DOWN,
		EVENT_KEY_UP,
		EVENT_MOUSE_MOVE,
		EVENT_MOUSE_DOWN,
		EVENT_MOUSE_UP
	} type;

	static InputEvent injectKeyDown(const OIS::KeyEvent &arg);
	static InputEvent injectKeyUp(const OIS::KeyEvent &arg);
	static InputEvent injectMouseMove(const OIS::MouseEvent &arg);
	static InputEvent injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	static InputEvent injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void send(InputController* obj);
};

class InputEvents
{
public:
	InputEvents(unsigned int timestep);
	~InputEvents();

	void injectInput(InputEvent event);

	void playback(InputController* obj);
	unsigned int getTimestep() { return mTimestep; }

private:
	unsigned int mTimestep;
	GQueue* mEvents;
};

class EventQueue
{
public:
	EventQueue();
	~EventQueue();

	void injectInput(InputEvent event);
	void setTimestep(unsigned int timestep);
	void playback(InputController* obj);
	
private:	
	GQueue* mEvents; //InputEvents
	InputEvents* current;
};

class InputRecorder
{
public:
	InputRecorder();
	~InputRecorder();

public:
	void injectKeyDown(const OIS::KeyEvent &arg);
	void injectKeyUp(const OIS::KeyEvent &arg);
	void injectMouseMove(const OIS::MouseEvent &arg);
	void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void setTimestep(unsigned int timestep);
	
private:
	unsigned int mTimestep;
	bool isRecording;

	EventQueue* mQueue;
};

class InputPlayer
{
public:
	InputPlayer();
	~InputPlayer();

public:
	void updateTimestep(unsigned int timestep);
	
private:
	GQueue* mEvents;
};


#endif
