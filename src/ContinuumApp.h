
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreConfigFile.h>

class ContinuumApp {
public:
	ContinuumApp ();
	virtual ~ContinuumApp ();

	int run();

private:
	int setup();
	void cleanup();
	static void setupResources();
	static void loadResources();
	bool configure();

private:
    static Ogre::String mResourcesCfg;
    static Ogre::String mPluginsCfg;

    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::Root* mRoot;
    Ogre::Camera* mCamera;
};
