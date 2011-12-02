
#include <stdbool.h>
#include <SDL/SDL.h>

#include "continuum.h"

int screenWidth = SCREEN_WIDTH;
int screenHeight = SCREEN_HEIGHT;

static SDL_Surface* screen = NULL;

void render()
{
	gl_render();
	SDL_GL_SwapBuffers();
}

void configure()
{
	if (screen)
	{
		SDL_FreeSurface(screen);
		screen = NULL;
	}
	int mode = SDL_OPENGL | SDL_RESIZABLE; //SDL_FULLSCREEN
	screen = SDL_SetVideoMode(screenWidth, screenHeight, 16, mode);
	gl_configure();
	render();
}

void handle_active_event(SDL_ActiveEvent* event)
{
	if (!event->gain)
	{
		printf("TODO pause\n");
	}
}

void handle_keyboard_event(SDL_KeyboardEvent* event)
{
	const char* state;
	if (event->state == SDL_PRESSED)
		state = "pressed";
	else
		state = "released";

	printf("%c key %s\n", event->keysym.sym, state);

}

void handle_mouse_motion_event(SDL_MouseMotionEvent* event)
{

}

void handle_mouse_button_event(SDL_MouseButtonEvent* event)
{
	const char* button;
	switch (event->button)
	{
		case SDL_BUTTON_LEFT:
			button = "left";
			break;
		case SDL_BUTTON_MIDDLE:
			button = "middle";
			break;
		case SDL_BUTTON_RIGHT:
			button = "right";
			break;
		default:
			button = "unknown";
			break;
	}

	const char* state;
	if (event->state == SDL_PRESSED)
		state = "pressed";
	else
		state = "released";

	printf("%s button %s at %d, %d\n", button, state, event->x, event->y);
}

void handle_resize_event(SDL_ResizeEvent* event)
{
	screenWidth = event->w;
	screenHeight = event->h;

	printf("Resizing to %d, %d\n", screenWidth, screenHeight);
	configure();
}

bool handle_event(SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_QUIT:
			return true;
		case SDL_ACTIVEEVENT:
			handle_active_event(&event->active);
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			handle_keyboard_event(&event->key);
			break;
		case SDL_MOUSEMOTION:
			handle_mouse_motion_event(&event->motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			handle_mouse_button_event(&event->button);
			break;
		case SDL_VIDEORESIZE:
			handle_resize_event(&event->resize);
			break;
			
	}
	return false;
}

void cleanup()
{
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	atexit(cleanup);

	SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	configure();
	SDL_Event event;

	bool done = false;
	while (!done)
	{
		render();
		while (SDL_PollEvent(&event))
		{
			if (!(done = handle_event(&event)))
				break;
		}
	}
	//TODO add a UI layer (clutter?)

	return 0;
}

