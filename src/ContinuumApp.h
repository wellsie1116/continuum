
#ifndef CONTINUUMAPP_H_6XPC8Z54
#define CONTINUUMAPP_H_6XPC8Z54

#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>

#include "World.h"
#include "PhysicsWorld.h"
#include "PlayerController.h"

class ContinuumApp
	: public Ogre::FrameListener
	, public Ogre::WindowEventListener
	, public OIS::KeyListener
	, public OIS::MouseListener
	, public OgreBites::SdkTrayListener
{
public:
	ContinuumApp ();
	~ContinuumApp ();

	int run();
	void requestLoadLevel(Ogre::String name);

private:
	int setup();
	static void setupResources();
	static void loadResources();
	bool configure();
	void createListeners();
	void cleanup();
	void loadScene(Ogre::String name);
	void createScene(Ogre::String name);

    // Ogre::FrameListener
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    bool frameStarted(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);

    // OIS::MouseListener
    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // Ogre::WindowEventListener
    void windowResized(Ogre::RenderWindow* rw);
    void windowClosed(Ogre::RenderWindow* rw);

private:
    static Ogre::String mResourcesCfg;
    static Ogre::String mPluginsCfg;

	Ogre::String mLoadLevel;
	bool mQuit;

	bool mTimeControl;

    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::Root* mRoot;
    Ogre::Camera* mCamera;

    OgreBites::SdkTrayManager* mTrayMgr;
    
	OIS::InputManager* mInputManager;
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;

	World* mWorld;
	PhysicsWorld* mPhysicsWorld;
    
	PlayerController* mPlayer;
};

#endif
