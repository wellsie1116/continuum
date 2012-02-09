
#include "ContinuumApp.h"
#include "PhysicsWorld.h"

#include "PhysicsState.h"

#define MAX_CONTACTS 50

extern ContinuumApp* app;

static void collideCallback(void *data, dGeomID o1, dGeomID o2);

struct PhysicsLink
{
	PhysicsLink()
		: sources(NULL)
		, sinks(NULL)
		, enabledTicks(0)
		, mark(false)
	{ }

	~PhysicsLink()
	{
		g_slist_free(sources);
		g_slist_free(sinks);
	}

	GSList* sources; //PhysicsObject
	GSList* sinks; //PhysicsObject

	unsigned int enabledTicks;
	bool mark;
};

struct PhysicsTeleport
{
	PhysicsTeleport(Ogre::String level, PhysicsObject* obj)
		: level(level)
		, obj(obj)
	{ }

	Ogre::String level;
	PhysicsObject* obj;
};

static void link_free(PhysicsLink* link);
static void link_disable(int id, PhysicsLink* link, gpointer user_data);
static void link_enable(int id, PhysicsLink* link, gpointer user_data);

PhysicsWorld::PhysicsWorld()
	: mWorld(NULL)
	, mSpace(NULL)
	, mContactGroup(NULL)
	, mObjects(NULL)
	, mSurfaces(NULL)
	, mTeleports(NULL)
	, mPlayer1(NULL)
	, mPlayer2(NULL)
{
	mLinks = g_hash_table_new_full(
			g_direct_hash,
			g_direct_equal,
			NULL,
			(GDestroyNotify)link_free
			);
}

PhysicsWorld::~PhysicsWorld()
{
	if (mWorld)
	{
		dWorldDestroy(mWorld);
		mWorld = NULL;
	}

	if (mSpace)
	{
		dSpaceDestroy(mSpace);
		mSpace = NULL;
	}
	
	if (mContactGroup)
	{
		dJointGroupDestroy(mContactGroup);
		mContactGroup = NULL;
	}

	if (mObjects)
	{
		g_slist_free_full(mObjects, (GDestroyNotify)physics_object_free);
		mObjects = NULL;
	}

	if (mSurfaces)
	{
		g_slist_free_full(mSurfaces, (GDestroyNotify)physics_object_free);
		mSurfaces = NULL;
	}

	g_hash_table_destroy(mLinks);

	for (GSList* pTeleports = mTeleports; pTeleports; pTeleports = pTeleports->next)
		delete (PhysicsTeleport*)pTeleports->data;
	g_slist_free(mTeleports);
	mTeleports = NULL;
}

void
PhysicsWorld::init()
{
	static bool init = false;
	if (!init)
	{
		dInitODE();
		init = true;
	}

	mWorld = dWorldCreate();
	mSpace = dHashSpaceCreate(0);
	dWorldSetGravity(mWorld, 0, -5.0, 0);
	mContactGroup = dJointGroupCreate(0);
}

void
PhysicsWorld::step()
{
	//setup forces
	for (GSList* pSurfaces = mSurfaces; pSurfaces; pSurfaces = pSurfaces->next)
	{
		PhysicsObject* obj = (PhysicsObject*)pSurfaces->data;
		obj->setupForces();
	}
	for (GSList* pObjects = mObjects; pObjects; pObjects = pObjects->next)
	{
		PhysicsObject* obj = (PhysicsObject*)pObjects->data;
		obj->sync();
		obj->setupForces();
	}

	//clear some values
	g_hash_table_foreach(mLinks, (GHFunc)link_disable, NULL);

	//step the world
	dSpaceCollide(mSpace, this, &collideCallback);
	dWorldQuickStep(mWorld, 1.0 / TICKS_PER_SECOND);
	dJointGroupEmpty(mContactGroup);

	//update some values
	g_hash_table_foreach(mLinks, (GHFunc)link_enable, NULL);
}

void
PhysicsWorld::sync()
{
	GSList* pObjects = mObjects;
	while (pObjects)
	{
		PhysicsObject* obj = (PhysicsObject*)pObjects->data;
		obj->sync();
		pObjects = pObjects->next;
	}
}
	
WorldObjectState* 
PhysicsWorld::save()
{ 
	return new WorldSnapshot(this);
}

void 
PhysicsWorld::restore(WorldObjectState* state)
{
	((WorldSnapshot*)state)->restore();
}

Box*
PhysicsWorld::createCompanionCube(Ogre::SceneNode* node)
{
	Box* cube = new CompanionCube(node, this);

	mObjects = g_slist_prepend(mObjects, cube);

	return cube;
}

Box*
PhysicsWorld::createBox(Ogre::SceneNode* node, float mass)
{
	Box* cube = new Box(node, this, mass);

	mObjects = g_slist_prepend(mObjects, cube);

	return cube;
}

Surface*
PhysicsWorld::createSurface(Ogre::SceneNode* node)
{
	Surface* surface = new Surface(node, this);

	mSurfaces = g_slist_prepend(mSurfaces, surface);

	return surface;
}

Player*
PhysicsWorld::createPlayer(Ogre::Camera* camera)
{
	Player* player = new Player(camera, this);

	mObjects = g_slist_prepend(mObjects, player);
	
	if (!mPlayer1)
	{
		mPlayer1 = player;
	}
	else if (!mPlayer2)
	{
		mPlayer2 = player;
	}
	else
	{
		g_assert_not_reached();
	}

	return player;
}

static PhysicsLink* getLink(GHashTable* links, int id)
{
	PhysicsLink* link = (PhysicsLink*)g_hash_table_lookup(links, GINT_TO_POINTER(id));

	if (!link)
	{
		link = new PhysicsLink();
		g_hash_table_insert(links, GINT_TO_POINTER(id), link);
	}

	return link;
}
	
void
PhysicsWorld::addLinkSource(int id, PhysicsObject* obj)
{
	PhysicsLink* link = getLink(mLinks, id);

	link->sources = g_slist_prepend(link->sources, obj);
}

void
PhysicsWorld::addLinkSink(int id, PhysicsObject* obj)
{
	PhysicsLink* link = getLink(mLinks, id);
	
	link->sinks = g_slist_prepend(link->sinks, obj);
}
	
void
PhysicsWorld::addTeleporter(Ogre::String name, PhysicsObject* obj)
{
	mTeleports = g_slist_prepend(mTeleports, new PhysicsTeleport(name, obj));
}

void
PhysicsWorld::nearCollide(dGeomID o1, dGeomID o2)
{
	int i;

	//stop the two players from colliding
	if (mPlayer1 && mPlayer2)
	{
		if (mPlayer1->contains(o1) && mPlayer2->contains(o2))
			return;
		if (mPlayer2->contains(o1) && mPlayer1->contains(o2))
			return;
	}

	// Get the dynamics body for each geom
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	// Create an array of dContact objects to hold the contact joints
	dContact contact[MAX_CONTACTS];

	// Now we set the joint properties of each contact. Going into the full
	// details here would require a tutorial of its own. I'll just say that the
	// members of the dContact structure control the joint behaviour, such as
	// friction, velocity and bounciness. See section 7.3.7 of the ODE manual
	// and have fun experimenting to learn more.
	for (i = 0; i < MAX_CONTACTS; i++)
	{
		if (!b1 || !b2)
		{
			contact[i].surface.mode = 0;
			contact[i].surface.mu = 16.0;
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = 0.01;
			contact[i].surface.bounce_vel = 0.01;
			contact[i].surface.soft_cfm = 0.01;
		}
		else
		{
			contact[i].surface.mode = dContactBounce | dContactSoftCFM;
			contact[i].surface.mu = 16.0;
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = 0.01;
			contact[i].surface.bounce_vel = 0.01;
			contact[i].surface.soft_cfm = 0.01;
		}
	}

	// Here we do the actual collision test by calling dCollide. It returns the
	// number of actual contact points or zero if there were none.
	int numc;
	if ((numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))))
	{
		//To add each contact point found to our joint group we call
		//dJointCreateContact which is just one of the many different joint
		//types available.
		for (i = 0; i < numc; i++)
		{
			dJointID c = dJointCreateContact(mWorld, mContactGroup, contact + i);
			dJointAttach(c, b1, b2);
		}

		if (!b1 ^ !b2)
		{
			dGeomID o = b1 ? o2 : o1;

			GHashTableIter it;
			int id;
			PhysicsLink* link;
			g_hash_table_iter_init(&it, mLinks);
			while (g_hash_table_iter_next(&it, (gpointer*)&id, (gpointer*)&link))
			{
				if (link->mark)
					continue;

				for (GSList* pSources = link->sources; pSources; pSources = pSources->next)
				{
					PhysicsObject* obj = (PhysicsObject*)pSources->data;

					if (obj->contains(o))
					{
						link->mark = true;
						break;
					}
				}
			}

			for (GSList* pTeleports = mTeleports; pTeleports; pTeleports = pTeleports->next)
			{
				PhysicsTeleport* teleport = (PhysicsTeleport*)pTeleports->data;
				if (teleport->obj->contains(o))
				{
					app->requestLoadLevel(teleport->level);
					break;
				}
			}
		}
	}
}

static void
collideCallback(void *data, dGeomID o1, dGeomID o2)
{
	((PhysicsWorld*)data)->nearCollide(o1, o2);
}

void link_free(PhysicsLink* link)
{
	delete link;
}

void
link_disable(int id, PhysicsLink* link, gpointer user_data)
{
	link->mark = false;
}

void
link_enable(int id, PhysicsLink* link, gpointer user_data)
{
	if (link->mark)
	{
		if (!link->enabledTicks)
		{
			printf("Link enabled: %d\n", id);
			for (GSList* pSinks = link->sinks; pSinks; pSinks = pSinks->next)
			{
				PhysicsObject* obj = (PhysicsObject*)pSinks->data;
				obj->setLinkEnabled(true);
			}
		}

		link->enabledTicks++;
	}
	else
	{
		if (link->enabledTicks)
		{
			printf("Link disabled: %d\n", id);
			for (GSList* pSinks = link->sinks; pSinks; pSinks = pSinks->next)
			{
				PhysicsObject* obj = (PhysicsObject*)pSinks->data;
				obj->setLinkEnabled(false);
			}
		}

		link->enabledTicks = 0;
	}
}

