
#include "ContinuumSceneLoader.h"

#include <OgreUserObjectBindings.h>
#include <OgreSceneNode.h>

ContinuumSceneLoader::ContinuumSceneLoader(PhysicsWorld* physicsWorld)
	: DotSceneLoader()
	, mPhysicsWorld(physicsWorld)
{ }
	
void
ContinuumSceneLoader::onEntityCreated(Ogre::Entity* pEntity, Ogre::SceneNode* pParentNode)
{
	Ogre::UserObjectBindings& bindings = pParentNode->getUserObjectBindings();

	Ogre::Any physType = bindings.getUserAny("Physics");
	if (!physType.isEmpty())
	{
		//create a weighted cube
		mPhysicsWorld->createCompanionCube(pParentNode);
	}
	else
	{
		//create a wall/floor/ceiling/etc
		mPhysicsWorld->createSurface(pParentNode);
	}
}
