

#include "ContinuumApp.h"

#include "continuum.h"

#include <stdio.h>
    
#if _DEBUG
Ogre::String ContinuumApp::mResourcesCfg = "resources.cfg";
Ogre::String ContinuumApp::mPluginsCfg = "plugins.cfg";
#else
Ogre::String ContinuumApp::mResourcesCfg = "resources.cfg";
Ogre::String ContinuumApp::mPluginsCfg = "plugins.cfg";
#endif

ContinuumApp::ContinuumApp()
	: mSceneMgr(NULL)
	, mWindow(NULL)
	, mRoot(NULL)
	, mCamera(NULL)
{
}

ContinuumApp::~ContinuumApp()
{
	if (mRoot)
	{
		delete mRoot;
	}
}

int ContinuumApp::run()
{
	int res = setup();
	if (res)
		return res;

    mRoot->startRendering();

    cleanup();
}

int ContinuumApp::setup()
{
    mRoot = new Ogre::Root(mPluginsCfg);

	setupResources();

	if (!configure())
		return 1;

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    mCamera = mSceneMgr->createCamera("PlayerCam");

    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	loadResources();
}

void ContinuumApp::setupResources()
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

void ContinuumApp::loadResources()
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

bool ContinuumApp::configure()
{
    if (mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        mWindow = mRoot->initialise(true, "Continuum");
        return true;
    }
    else
    {
        return false;
    }
}

void ContinuumApp::cleanup()
{
}

