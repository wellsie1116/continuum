
#ifndef CONTINUUMSCENELOADER_H_88JESOGV
#define CONTINUUMSCENELOADER_H_88JESOGV

#include "DotSceneLoader.h"
#include "PhysicsWorld.h"

class ContinuumSceneLoader : public Ogre::DotSceneLoader{
public:
	ContinuumSceneLoader(PhysicsWorld* physicsWorld);

protected:
	virtual void onEntityCreated(Ogre::Entity* pEntity, Ogre::SceneNode* pParentNode);

private:
	PhysicsWorld* mPhysicsWorld;
};

#endif
