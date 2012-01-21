
#include "ContinuumSceneLoader.h"

#include <OgreUserObjectBindings.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

ContinuumSceneLoader::ContinuumSceneLoader(PhysicsWorld* physicsWorld)
	: DotSceneLoader()
	, mPhysicsWorld(physicsWorld)
{ }
	
void
ContinuumSceneLoader::onEntityCreated(Ogre::Entity* pEntity, Ogre::SceneNode* pParentNode)
{
	Ogre::UserObjectBindings& bindings = pParentNode->getUserObjectBindings();
	pEntity->setCastShadows(true);

	Ogre::Any physType = bindings.getUserAny("Physics");
	if (!physType.isEmpty())
	{
		Ogre::Any mass = bindings.getUserAny("Mass");
		if (mass.isEmpty())
		{
			//create a weighted cube
			mPhysicsWorld->createCompanionCube(pParentNode);
		}
		else
		{
			//create a weighted cube
			mPhysicsWorld->createBox(pParentNode, Ogre::any_cast<double>(mass));
		}
	}
	else
	{
		//create a wall/floor/ceiling/etc
		mPhysicsWorld->createSurface(pParentNode);
	}
}

