
#include <SDL/SDL_opengl.h>

#include "continuum.h"

void gl_configure()
{
	glViewport(0, 0, screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, screenHeight, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void gl_render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex2i(screenWidth / 2, screenHeight / 8);

		glColor3f(0.0, 1.0, 0.0);
		glVertex2i(screenWidth / 4, 7 * screenHeight / 8);

		glColor3f(0.0, 0.0, 1.0);
		glVertex2i(3 * screenWidth / 4, 7 * screenHeight / 8);
	}
	glEnd();
}
