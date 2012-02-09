
#include "PlayerController.h"
	
#define PLAYER_SCALE .020

PlayerController::PlayerController(Player* player)
	: mPlayer(player)
{
	mCamera = mPlayer->getCamera();
	mSceneMgr = mCamera->getSceneManager();

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
	mCameraMan->setTopSpeed(50);

	setupModel();
	setupCamera();
	setupAnimations();
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
	//return mCameraMan->frameRenderingQueued(evt);
	addTime(evt.timeSinceLastFrame);
	return true;
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
PlayerController::remove()
{
	mSceneMgr->destroySceneNode(mPlayerNode);
	mSceneMgr->destroyCamera(mCamera);
}

void
PlayerController::setupModel()
{
	Ogre::SceneNode* root = mSceneMgr->getRootSceneNode();

	Ogre::Vector3 playerPosition;
	playerPosition = mCamera->getPosition();
	mPlayerNode = root->createChildSceneNode(playerPosition);
	mPlayerEntity = mSceneMgr->createEntity("robot.mesh");
	{
		Ogre::SceneNode* modelNode = mPlayerNode->createChildSceneNode();
		modelNode->attachObject(mPlayerEntity);
		modelNode->yaw(Ogre::Radian(-M_PI/2.0));
		//printf("Bounding Radius: %f\n", mPlayerEntity->getBoundingRadius());
		modelNode->scale(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);
		//printf("Bounding Radius: %f\n", mPlayerEntity->getBoundingRadius());
	}
	mPlayer->setNode(mPlayerNode);
}

void
PlayerController::setupCamera()
{
}
	
void
PlayerController::setupAnimations()
{
	Ogre::String animName[] = {"Idle", "Walk", "Die", "Shoot", "Slump"};

	for (int i = 0; i < ANIM_COUNT; i++)
	{
		mAnims[i] = mPlayerEntity->getAnimationState(animName[i]);
		mAnims[i]->setLoop(true);
	}

	mAnims[ANIM_IDLE]->setEnabled(true);
	mAnims[ANIM_IDLE]->setWeight(0.1);
	mAnims[ANIM_WALK]->setEnabled(true);
	mAnims[ANIM_WALK]->setWeight(0.1);
}
	
void
PlayerController::addTime(Ogre::Real delta)
{

	for (int i = 0; i < ANIM_COUNT; i++)
	{
		switch (i)
		{
			case ANIM_WALK:
			{
				Ogre::Real d = delta;
				double vel = mPlayer->getForwardVelocity();
				mAnims[ANIM_WALK]->setWeight(vel);

				if (vel < 0.0)
					d *= -1;

				if (abs(vel) > 5)
					d *= 1+(abs(vel) - 5.0) * 0.1;

				mAnims[i]->addTime(d);
			} break;
			default:
				mAnims[i]->addTime(delta);
		}
	}

}


