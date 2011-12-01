
#include <SDL/SDL_opengl.h>

#include "continuum.h"

void gl_configure()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, screenHeight, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void gl_render()
{
	glBegin(GL_TRIANGLES);
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex2i(300, 50);

		glColor3f(0.0, 1.0, 0.0);
		glVertex2i(100, 500);

		glColor3f(0.0, 0.0, 1.0);
		glVertex2i(500, 500);
	}
	glEnd();
}
