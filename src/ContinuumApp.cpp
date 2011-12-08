

#include "ContinuumApp.h"

#include "continuum.h"

#include <stdio.h>
    
#if _DEBUG
Ogre::String ContinuumApp::mResourcesCfg = "resources_d.cfg";
Ogre::String ContinuumApp::mPluginsCfg = "plugins_d.cfg";
#else
Ogre::String ContinuumApp::mResourcesCfg = "resources.cfg";
Ogre::String ContinuumApp::mPluginsCfg = "plugins.cfg";
#endif

ContinuumApp::ContinuumApp()
	: mQuit(false)
	, mSceneMgr(NULL)
	, mWindow(NULL)
	, mRoot(NULL)
	, mCamera(NULL)
	, mTrayMgr(NULL)
	, mInputManager(NULL)
	, mMouse(NULL)
	, mKeyboard(NULL)
{
}

ContinuumApp::~ContinuumApp()
{
	if (mTrayMgr) delete mTrayMgr;

    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);

	if (mRoot) delete mRoot;
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

	//TODO load scene
	
	createListeners();
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

void ContinuumApp::createListeners()
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
    
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    mRoot->addFrameListener(this);
}
    
bool ContinuumApp::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if (mWindow->isClosed())
        return false;

    if (mQuit)
        return false;

    mKeyboard->capture();
    mMouse->capture();
    
	mTrayMgr->frameRenderingQueued(evt);

	return true;
}

bool ContinuumApp::keyPressed(const OIS::KeyEvent &arg)
{
    if (mTrayMgr->isDialogVisible()) return true;

	switch (arg.key)
	{
		case OIS::KC_ESCAPE:
			mQuit = true;
			break;
		default:
			return false;
    }

	return true;
}

bool ContinuumApp::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}

bool ContinuumApp::mouseMoved(const OIS::MouseEvent &arg)
{
    if (mTrayMgr->injectMouseMove(arg)) return true;

	return true;
}

bool ContinuumApp::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;

	return true;
}

bool ContinuumApp::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;

	return true;
}

void ContinuumApp::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void ContinuumApp::windowClosed(Ogre::RenderWindow* rw)
{
    if (rw == mWindow)
    {
        if (mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = NULL;
        }
    }
}


void ContinuumApp::cleanup()
{
}

