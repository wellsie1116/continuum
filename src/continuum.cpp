
#include "continuum.h"
#include "ContinuumApp.h"

int main(int argc, char *argv[])
{
	ContinuumApp app;

	try
	{
		return app.run();
	}
	catch (Ogre::Exception& e)
	{
		printf("An exception has occurred: %s\n", e.getFullDescription().c_str());
		return 1;
	}
}

