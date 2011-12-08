
#include <SDL/SDL_opengl.h>

#include "ContinuumApp.h"
#include "continuum.h"

#define AXIS_LENGTH 100.0f
static int dspAxes; 

void gl_configure()
{
	glViewport(0, 0, screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)screenWidth / (float)screenHeight, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void gl_render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(-10.0f, -10.0f, 10.0f,
	          0.0f, 0.0f, 0.0f,
	          0.0f, 0.0f, 1.0f);

	//TODO render game
	
#if DEBUG_RENDER
	glCallList(dspAxes);
#endif

	//switch to ortho rendering
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, screenHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

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

	continuum_app_render_ui(continuum_app_get_instance());
	
	//restore our state
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

int
gl_init()
{
#if DEBUG_RENDER
	dspAxes = glGenLists(1);
	glNewList(dspAxes, GL_COMPILE);
	{
		//glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		{
			glBegin(GL_LINES);
			{
				glLineWidth(4.0f);
				glColor3f(0.5f, 0.0f, 0.0f);
				glVertex3i(-AXIS_LENGTH, 0, 0);
				glVertex3i(0, 0, 0);
				glColor3f(0.0f, 0.5f, 0.0f);
				glVertex3i(0, -AXIS_LENGTH, 0);
				glVertex3i(0, 0, 0);
				glColor3f(0.0f, 0.0f, 0.5f);
				glVertex3i(0, 0, -AXIS_LENGTH);
				glVertex3i(0, 0, 0);

				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3i(0, 0, 0);
				glVertex3i(AXIS_LENGTH, 0, 0);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3i(0, 0, 0);
				glVertex3i(0, AXIS_LENGTH, 0);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3i(0, 0, 0);
				glVertex3i(0, 0, AXIS_LENGTH);

			}
			glEnd();
		}
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_LIGHTING);
	}
	glEndList();

#endif

	return 0;
}

