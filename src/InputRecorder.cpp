
#include "InputRecorder.h"

InputEvent
InputEvent::injectKeyDown(const OIS::KeyEvent &arg)
{
	InputEvent res;
	res.device = (OIS::Object*)arg.device;
	res.event.key.key = arg.key;
	res.event.key.text = arg.text;
	res.type = EVENT_KEY_DOWN;
	return res;
}

InputEvent
InputEvent::injectKeyUp(const OIS::KeyEvent &arg)
{
	InputEvent res;
	res.device = (OIS::Object*)arg.device;
	res.event.key.key = arg.key;
	res.event.key.text = arg.text;
	res.type = EVENT_KEY_UP;
	return res;
}

void copy_axis(const OIS::Axis& in, Axis* out)
{
	out->abs = in.abs;
	out->rel = in.rel;
	out->absOnly = in.absOnly;
}

InputEvent
InputEvent::injectMouseMove(const OIS::MouseEvent &arg)
{
	InputEvent res;
	res.device = (OIS::Object*)arg.device;
	res.event.mouse.state.width = arg.state.width;
	res.event.mouse.state.height = arg.state.height;
	res.event.mouse.state.buttons = arg.state.buttons;
	copy_axis(arg.state.X, &res.event.mouse.state.X);
	copy_axis(arg.state.Y, &res.event.mouse.state.Y);
	copy_axis(arg.state.Z, &res.event.mouse.state.Z);
	res.type = EVENT_MOUSE_MOVE;
	return res;
}

InputEvent
InputEvent::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	InputEvent res;
	res.device = (OIS::Object*)arg.device;
	res.event.mouse.state.width = arg.state.width;
	res.event.mouse.state.height = arg.state.height;
	res.event.mouse.state.buttons = arg.state.buttons;
	copy_axis(arg.state.X, &res.event.mouse.state.X);
	copy_axis(arg.state.Y, &res.event.mouse.state.Y);
	copy_axis(arg.state.Z, &res.event.mouse.state.Z);
	res.event.mouse.id = id;
	res.type = EVENT_MOUSE_DOWN;
	return res;
}

InputEvent
InputEvent::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	InputEvent res;
	res.device = (OIS::Object*)arg.device;
	res.event.mouse.state.width = arg.state.width;
	res.event.mouse.state.height = arg.state.height;
	res.event.mouse.state.buttons = arg.state.buttons;
	copy_axis(arg.state.X, &res.event.mouse.state.X);
	copy_axis(arg.state.Y, &res.event.mouse.state.Y);
	copy_axis(arg.state.Z, &res.event.mouse.state.Z);
	res.event.mouse.id = id;
	res.type = EVENT_MOUSE_UP;
	return res;
}

void
Axis::init(OIS::Axis* axis)
{
	axis->abs = abs;
	axis->rel = rel;
	axis->absOnly = absOnly;
}

void
MouseState::init(OIS::MouseState* state)
{
	state->width = width;
	state->height = height;
	state->buttons = buttons;
	X.init(&state->X);
	Y.init(&state->Y);
	Z.init(&state->Z);
}

void
InputEvent::send(InputController* obj)
{
	switch (type)
	{
		case EVENT_KEY_DOWN:
		{
			OIS::KeyEvent arg(device, event.key.key, event.key.text);
			obj->injectKeyDown(arg);
		} break;
		case EVENT_KEY_UP:
		{
			OIS::KeyEvent arg(device, event.key.key, event.key.text);
			obj->injectKeyUp(arg);
		} break;
		case EVENT_MOUSE_MOVE:
		{
			OIS::MouseState state;
			event.mouse.state.init(&state);
			OIS::MouseEvent arg(device, state);
			obj->injectMouseMove(arg);
		} break;
		case EVENT_MOUSE_DOWN:
		{
			OIS::MouseState state;
			event.mouse.state.init(&state);
			OIS::MouseEvent arg(device, state);
			obj->injectMouseDown(arg, event.mouse.id);
		} break;
		case EVENT_MOUSE_UP:
		{
			OIS::MouseState state;
			event.mouse.state.init(&state);
			OIS::MouseEvent arg(device, state);
			obj->injectMouseUp(arg, event.mouse.id);
		} break;
	}
}

static void free_event(InputEvent* event, gpointer user_data)
{
	delete event;
}

InputEvents::InputEvents(unsigned int timestep)
	: mTimestep(timestep)
{
	mEvents = g_queue_new();
}

InputEvents::~InputEvents()
{
	g_queue_foreach(mEvents, (GFunc)free_event, NULL);
	g_queue_free(mEvents);
}

void
InputEvents::injectInput(InputEvent event)
{
	InputEvent* obj = new InputEvent(event);
	g_queue_push_tail(mEvents, obj);
}

void
InputEvents::playback(InputController* obj)
{
	for (GList* pEvents = mEvents->head; pEvents; pEvents = pEvents->next)
	{
		InputEvent* event = (InputEvent*)pEvents->data;
		event->send(obj);
	}
}

static void free_events(InputEvents* events, gpointer user_data)
{
	delete events;
}

EventQueue::EventQueue()
	: current(NULL)
{
	mEvents = g_queue_new();
}

EventQueue::~EventQueue()
{
	g_queue_foreach(mEvents, (GFunc)free_events, NULL);
	g_queue_free(mEvents);
}

void
EventQueue::injectInput(InputEvent event)
{
	if (!current)
		return;

	InputEvents* events = (InputEvents*)g_queue_peek_tail(mEvents);
	events->injectInput(event);
}

void
EventQueue::setTimestep(unsigned int timestep)
{
	for (GList* pEvents = mEvents->tail; pEvents; pEvents = pEvents->prev)
	{
		InputEvents* events = (InputEvents*)pEvents->data;
		if (events->getTimestep() == timestep)
		{
			//found it
			current = events;
			return;
		}
		else if (events->getTimestep() > timestep)
		{
			if (pEvents->next)
			{
				//somewhere in the middle, don't make it
				current = NULL;
			}
			else
			{
				//append a new set of input events 
				current = new InputEvents(timestep);
				g_queue_push_tail(mEvents, events);
			}
			return;
		}
	}

	//does not exist
	current = NULL;
}

void
EventQueue::playback(InputController* obj)
{
	if (!current)
		return;

	current->playback(obj);
}

InputRecorder::InputRecorder()
{
	mQueue = new EventQueue();
}

InputRecorder::~InputRecorder()
{
	delete mQueue;
}

void
InputRecorder::injectKeyDown(const OIS::KeyEvent &arg)
{
	mQueue->injectInput(InputEvent::injectKeyDown(arg));
}

void
InputRecorder::injectKeyUp(const OIS::KeyEvent &arg)
{
	mQueue->injectInput(InputEvent::injectKeyUp(arg));
}

void
InputRecorder::injectMouseMove(const OIS::MouseEvent &arg)
{
	mQueue->injectInput(InputEvent::injectMouseMove(arg));
}

void
InputRecorder::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	mQueue->injectInput(InputEvent::injectMouseDown(arg, id));
}

void
InputRecorder::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	mQueue->injectInput(InputEvent::injectMouseUp(arg, id));
}

void
InputRecorder::setTimestep(unsigned int timestep)
{
	mQueue->setTimestep(timestep);
}
	
void
InputRecorder::playback(InputController* obj)
{
	mQueue->playback(obj);
}

