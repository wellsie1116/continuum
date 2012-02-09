
#ifndef PLAYERCONTROLLER_H_455N0KZM
#define PLAYERCONTROLLER_H_455N0KZM

#include <Ogre.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkCameraMan.h>

#include "WorldObject.h"
#include "Player.h"

enum AnimID
{
	ANIM_IDLE,
	ANIM_WALK,
	ANIM_DIE,
	ANIM_SHOOT,
	ANIM_SLUMP,

	ANIM_COUNT
};


class PlayerController : public InputController
{
public:
    PlayerController(Player* player);
	~PlayerController();

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void injectKeyDown(const OIS::KeyEvent &arg);
	void injectKeyUp(const OIS::KeyEvent &arg);
	void injectMouseMove(const OIS::MouseEvent &arg);
	void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	Player* getPlayer() { return mPlayer; }

private:
	void setupModel();
	void setupCamera();
	void setupAnimations();

	void addTime(Ogre::Real delta);

private:
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	Ogre::SceneNode* mPlayerNode;
	Ogre::Entity* mPlayerEntity;
	Ogre::AnimationState* mAnims[ANIM_COUNT];

	Player* mPlayer;
    OgreBites::SdkCameraMan* mCameraMan;
};

#endif
