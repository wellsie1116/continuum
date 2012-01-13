
#include "PlayerController.h"

PlayerController::PlayerController(Player* player)
	: mPlayer(player)
{
	mCamera = mPlayer->getCamera();
	mSceneMgr = mCamera->getSceneManager();

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
	return true;
	//return mCameraMan->frameRenderingQueued(evt);
}

void
PlayerController::injectKeyDown(const OIS::KeyEvent &arg)
{
	//mCameraMan->injectKeyDown(arg);
	mPlayer->injectKeyDown(arg);
}

void
PlayerController::injectKeyUp(const OIS::KeyEvent &arg)
{
	//mCameraMan->injectKeyUp(arg);
	mPlayer->injectKeyUp(arg);
}

void
PlayerController::injectMouseMove(const OIS::MouseEvent &arg)
{
	//mCameraMan->injectMouseMove(arg);
	mPlayer->injectMouseMove(arg);
}

void
PlayerController::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	//mCameraMan->injectMouseDown(arg, id);
}

void
PlayerController::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	//mCameraMan->injectMouseUp(arg, id);
}

void
PlayerController::setupModel()
{
	Ogre::SceneNode* root = mSceneMgr->getRootSceneNode();

	Ogre::Vector3 playerPosition;
	//FIXME: fixed camera position to be global
	playerPosition = mCamera->getPosition();
	mPlayerNode = root->createChildSceneNode(playerPosition);
	mPlayerEntity = mSceneMgr->createEntity("SinbadBody", "Sinbad.mesh");
	mPlayerNode->attachObject(mPlayerEntity);
	mPlayer->setNode(mPlayerNode);
}

void
PlayerController::setupCamera()
{
}

