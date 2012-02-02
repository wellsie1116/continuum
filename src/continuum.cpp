
#include "continuum.h"
#include "ContinuumApp.h"

ContinuumApp* app;

int main(int argc, char *argv[])
{
	app = new ContinuumApp();
	try
	{
		int res = app->run();
		delete app;
		return res;
	}
	catch (Ogre::Exception& e)
	{
		printf("An exception has occurred: %s\n", e.getFullDescription().c_str());
		return 1;
	}
}

