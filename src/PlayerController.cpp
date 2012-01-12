
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

	switch (arg.key)
	{
		case OIS::KC_W:
			mPlayer->startMove(FORWARD);
			break;
		case OIS::KC_S:
			mPlayer->startMove(BACKWARD);
			break;
		case OIS::KC_A:
			mPlayer->startMove(LEFT);
			break;
		case OIS::KC_D:
			mPlayer->startMove(RIGHT);
			break;
    }
}

void
PlayerController::injectKeyUp(const OIS::KeyEvent &arg)
{
	//mCameraMan->injectKeyUp(arg);

	switch (arg.key)
	{
		case OIS::KC_W:
			mPlayer->stopMove(FORWARD);
			break;
		case OIS::KC_S:
			mPlayer->stopMove(BACKWARD);
			break;
		case OIS::KC_A:
			mPlayer->stopMove(LEFT);
			break;
		case OIS::KC_D:
			mPlayer->stopMove(RIGHT);
			break;
    }
}

void
PlayerController::injectMouseMove(const OIS::MouseEvent &arg)
{
	mCameraMan->injectMouseMove(arg);
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
	playerPosition = mCamera->getParentSceneNode()->getPosition() + Ogre::Vector3::UNIT_Y * 5;
	mPlayerNode = root->createChildSceneNode(playerPosition);
	mPlayerEntity = mSceneMgr->createEntity("SinbadBody", "Sinbad.mesh");
	mPlayerNode->attachObject(mPlayerEntity);
}

void
PlayerController::setupCamera()
{
}

