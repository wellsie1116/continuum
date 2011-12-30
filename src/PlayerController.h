
#ifndef PLAYERCONTROLLER_H_455N0KZM
#define PLAYERCONTROLLER_H_455N0KZM

#include <Ogre.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkCameraMan.h>

class PlayerController
{
public:
    PlayerController(Ogre::Camera* camera);
	~PlayerController();

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void injectKeyDown(const OIS::KeyEvent &arg);
	void injectKeyUp(const OIS::KeyEvent &arg);
	void injectMouseMove(const OIS::MouseEvent &arg);
	void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

private:
	void setupModel();
	void setupCamera();

private:
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	Ogre::SceneNode* mPlayerNode;
	Ogre::Entity* mPlayer;

    OgreBites::SdkCameraMan* mCameraMan;
};

#endif
