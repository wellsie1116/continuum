
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

bool handle_event(SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_QUIT:
			return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_OPENGL | SDL_FULLSCREEN);
	SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_OPENGL);
	SDL_Event event;

	bool done = false;
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (!(done = handle_event(&event)))
				break;
		}
	}
	//TODO add a UI layer (clutter?)


	SDL_FreeSurface(screen);

	SDL_Quit();
	
	return 0;
}

