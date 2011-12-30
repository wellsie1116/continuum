
#include "PlayerController.h"

PlayerController::PlayerController(Ogre::Camera* camera)
	: mCamera(camera)
{
	mSceneMgr = camera->getSceneManager();

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
	mCameraMan->setTopSpeed(50);

	setupModel();
	setupCamera();
}

PlayerController::~PlayerController()
{
	if (mCameraMan)
	{
		delete mCameraMan;
		mCameraMan = NULL;
	}
}

bool
PlayerController::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	return mCameraMan->frameRenderingQueued(evt);
}

void
PlayerController::injectKeyDown(const OIS::KeyEvent &arg)
{
	mCameraMan->injectKeyDown(arg);
}

void
PlayerController::injectKeyUp(const OIS::KeyEvent &arg)
{
	mCameraMan->injectKeyUp(arg);
}

void
PlayerController::injectMouseMove(const OIS::MouseEvent &arg)
{
	mCameraMan->injectMouseMove(arg);
}

void
PlayerController::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	mCameraMan->injectMouseDown(arg, id);
}

void
PlayerController::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	mCameraMan->injectMouseUp(arg, id);
}

void
PlayerController::setupModel()
{
	Ogre::SceneNode* root = mSceneMgr->getRootSceneNode();

	Ogre::Vector3 playerPosition;
	playerPosition = mCamera->getParentSceneNode()->getPosition() + Ogre::Vector3::UNIT_Y * 5;
	mPlayerNode = root->createChildSceneNode(playerPosition);
	mPlayer = mSceneMgr->createEntity("SinbadBody", "Sinbad.mesh");
	mPlayerNode->attachObject(mPlayer);
}

void
PlayerController::setupCamera()
{
}

