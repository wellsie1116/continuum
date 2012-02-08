
#include "ContinuumApp.h"

#include "continuum.h"
#include "ContinuumSceneLoader.h"

#include <OgreLog.h>

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
	, mLoadLevel("")
	, mTimeControl(false)
	, mSceneMgr(NULL)
	, mWindow(NULL)
	, mRoot(NULL)
	, mCamera(NULL)
	, mTrayMgr(NULL)
	, mInputManager(NULL)
	, mMouse(NULL)
	, mKeyboard(NULL)
	, mWorld(NULL)
	, mPhysicsWorld(NULL)
	, mPlayer(NULL)
{
}

ContinuumApp::~ContinuumApp()
{
	if (mTrayMgr) delete mTrayMgr;
	if (mPlayer) delete mPlayer;

    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);

	if (mRoot) delete mRoot;
	
	if (mWorld)
	{
		delete mWorld;
		mWorld = NULL;
	}
	
	if (mPhysicsWorld)
	{
		delete mPhysicsWorld;
		mPhysicsWorld = NULL;
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
	
void
ContinuumApp::requestLoadLevel(Ogre::String name)
{
	mLoadLevel = name;
}

int ContinuumApp::setup()
{
	//disable debug output
	Ogre::LogManager* logManager = new Ogre::LogManager();
	Ogre::Log* defaultLog = new Ogre::Log("debug.log", false);
	logManager->setDefaultLog(defaultLog);

    mRoot = new Ogre::Root(mPluginsCfg);

	setupResources();

	if (!configure())
		return 1;

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	loadResources();

	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	//mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	//mSceneMgr->setShadowTextureSize(1024);
	//mSceneMgr->setShadowTextureCount(1);

	loadScene("menu");
	
	createListeners();

	return 0;
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

	if (mLoadLevel != "")
	{
		loadScene(mLoadLevel);
		mLoadLevel = "";
	}

    mKeyboard->capture();
    mMouse->capture();
    
	mTrayMgr->frameRenderingQueued(evt);

	if (!mTrayMgr->isDialogVisible())
	{
        mPlayer->frameRenderingQueued(evt);
	}

	return true;
}

bool ContinuumApp::frameStarted(const Ogre::FrameEvent& evt)
{
	if (mLoadLevel != "")
		return true;

	mWorld->step();
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
		case OIS::KC_LMENU:
			mWorld->freezeTime();
			mTimeControl = true;
			break;
		case OIS::KC_P:
			mWindow->writeContentsToFile("screenshot.png");
			break;
		default:
			mWorld->injectKeyDown(arg);
			return false;
    }
    

	return true;
}

bool ContinuumApp::keyReleased(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{
		case OIS::KC_LMENU:
			mWorld->resumeTime();
			mTimeControl = false;
			break;
		default:
			mWorld->injectKeyUp(arg);
			return false;
    }

	return true;
}

bool ContinuumApp::mouseMoved(const OIS::MouseEvent &arg)
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
    
	mWorld->injectMouseMove(arg);

	if (mTimeControl)
	{
		if (arg.state.Z.rel > 0)
		{
			mWorld->accelerateTime();
		}
		else if (arg.state.Z.rel < 0)
		{
			mWorld->decelerateTime();
		}
	}

	return true;
}

bool ContinuumApp::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
	
	switch (id)
	{
		case 0:
			//Set marker
			mWorld->setMarker();
			break;
		case 1:
			//Jump back
			mWorld->jumpMarker();
			break;
		case 2:
			//freeze time
			mWorld->freezeTime();
			break;
		default:
			mWorld->injectMouseDown(arg, id);

	}
	return true;
}

bool ContinuumApp::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;

	mWorld->injectMouseUp(arg, id);

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

void printSceneGraph(Ogre::Node* root, int indent = 0)
{
	for (int i = 0; i < indent; i++)
	{
		printf("  ");
	}
	printf("N: %s\n", root->getName().c_str());

	Ogre::Node::ChildNodeIterator it = root->getChildIterator();
	while (it.hasMoreElements())
	{
		Ogre::Node* node = it.getNext();
		printSceneGraph(node, indent + 1);
	}
}

void ContinuumApp::loadScene(Ogre::String name)
{
	name = name + ".scene";
	printf("Loading level: %s\n", name.c_str());
	if (mWorld)
	{
		delete mWorld;
		mWorld = NULL;
	}
	if (mPhysicsWorld)
	{
		delete mPhysicsWorld;
		mPhysicsWorld = NULL;
	}

	mWindow->removeAllViewports();
	mSceneMgr->destroyAllCameras();
	mSceneMgr->clearScene();
	printf("Empty scene\n");
	printSceneGraph(mSceneMgr->getRootSceneNode());
	printf("\n");

	mWorld = new World();
	mPhysicsWorld = new PhysicsWorld();
	mPhysicsWorld->init();

	mWorld->addObject(mPhysicsWorld);

	createScene(name);
	mWorld->addInputController(mPlayer);

	mWorld->start();
	printf("Loaded level: %s\n", name.c_str());
}

void ContinuumApp::createScene(Ogre::String name)
{
	//load the scene
	Ogre::SceneNode* scene = mSceneMgr->getRootSceneNode()->createChildSceneNode("Scene");
	ContinuumSceneLoader loader(mPhysicsWorld);
	if (!loader.parseDotScene(name, "General", mSceneMgr, scene))
	{
		printf("Error loading scene file: %s\n", name.c_str());
		exit(1);
	}
	printSceneGraph(mSceneMgr->getRootSceneNode());

	//get some objects from it
	if (mSceneMgr->hasCamera("Camera"))
	{
		mCamera = mSceneMgr->getCamera("Camera");
	}
	else
	{
		mCamera = mSceneMgr->getCameraIterator().getNext();
	}
	{
		Ogre::Vector3 pos = mCamera->getRealPosition();
		Ogre::Quaternion orientation = mCamera->getRealOrientation();
		mCamera->getParentSceneNode()->detachObject(mCamera);
		mSceneMgr->getRootSceneNode()->attachObject(mCamera);
		mCamera->setPosition(pos);
		mCamera->setOrientation(orientation);
	}
	Player* player = mPhysicsWorld->createPlayer(mCamera);
	if (mPlayer)
	{
		delete mPlayer;
		mPlayer = NULL;
	}
	mPlayer = new PlayerController(player);

	//setup our viewport
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0.1, 0.1, 0.1));
	Ogre::Real width = vp->getActualWidth();
	Ogre::Real height = vp->getActualHeight();
    mCamera->setAspectRatio(width / height);
}

