
#include "ContinuumSceneLoader.h"

#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>
#include <OgreUserObjectBindings.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

static int anyToInt(Ogre::Any& any);

ContinuumSceneLoader::ContinuumSceneLoader(PhysicsWorld* physicsWorld)
	: DotSceneLoader()
	, mPhysicsWorld(physicsWorld)
{ }
	
	
int
anyToInt(Ogre::Any& any)
{
	if (any.getType() == typeid(Ogre::String))
	{
		return atoi(Ogre::any_cast<Ogre::String>(any).c_str());
	}
	else if (any.getType() == typeid(double))
	{
		return (int)Ogre::any_cast<double>(any);
	}
	else
	{
		return Ogre::any_cast<int>(any);
	}
}

void
ContinuumSceneLoader::onEntityCreated(Ogre::Entity* pEntity, Ogre::SceneNode* pParentNode)
{
	Ogre::UserObjectBindings& bindings = pParentNode->getUserObjectBindings();

	PhysicsObject* obj = NULL;
	
	Ogre::Any physType = bindings.getUserAny("Physics");
	if (!physType.isEmpty())
	{
		Ogre::String type = Ogre::any_cast<Ogre::String>(physType);
		if (type != "no")
		{
			Ogre::Any mass = bindings.getUserAny("Mass");
			if (mass.isEmpty())
			{
				//create a weighted cube
				obj = mPhysicsWorld->createCompanionCube(pParentNode);
			}
			else
			{
				//create a weighted cube with a given mass
				obj = mPhysicsWorld->createBox(pParentNode, Ogre::any_cast<double>(mass));
			}
		}
		else
		{
			Ogre::SceneNode::ObjectIterator it = pParentNode->getAttachedObjectIterator();
			while (it.hasMoreElements())
			{
				Ogre::MovableObject* o = it.getNext();
				o->setCastShadows(false);
			}
		}
	}
	else
	{
		//create a wall/floor/ceiling/etc
		obj = mPhysicsWorld->createSurface(pParentNode);
	}
	
	if (obj)
	{
		Ogre::Any linkSource = bindings.getUserAny("LinkSource");
		if (!linkSource.isEmpty())
		{
			int id = anyToInt(linkSource);
			mPhysicsWorld->addLinkSource(id, obj);
		}

		Ogre::Any linkSink = bindings.getUserAny("LinkSink");
		if (!linkSink.isEmpty())
		{
			int id = anyToInt(linkSink);
			mPhysicsWorld->addLinkSink(id, obj);
		}
		
		Ogre::Any teleport = bindings.getUserAny("Teleport");
		if (!teleport.isEmpty())
		{
			Ogre::String dest = Ogre::any_cast<Ogre::String>(teleport);
			mPhysicsWorld->addTeleporter(dest, obj);
		}
	}

	//ps = sceneMgr->createParticleSystem("Aureola", "Examples/Aureola");
	//ps = sceneMgr->createParticleSystem("Nimbus", "Examples/GreenyNimbus");
	Ogre::SceneManager* sceneMgr = pParentNode->getCreator();
	Ogre::Any particles1 = bindings.getUserAny("Particles1");
	if (!particles1.isEmpty())
	{
		Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);
		Ogre::String name = Ogre::any_cast<Ogre::String>(particles1);
		Ogre::String psName = pParentNode->getName() + "-" + name;
		Ogre::ParticleSystem* ps;
		ps = sceneMgr->createParticleSystem(psName, name);
		pParentNode->attachObject(ps);
	}
	Ogre::Any particles2 = bindings.getUserAny("Particles2");
	if (!particles2.isEmpty())
	{
		Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);
		Ogre::String name = Ogre::any_cast<Ogre::String>(particles2);
		Ogre::String psName = pParentNode->getName() + "-" + name;
		Ogre::ParticleSystem* ps;
		ps = sceneMgr->createParticleSystem(psName, name);
		pParentNode->attachObject(ps);
	}

}

