
#ifndef BOX_H_QNH37BLD
#define BOX_H_QNH37BLD

#include <OgreSceneNode.h>

#include <Newton.h>

class PhysicsWorld;

class Box {
public:
	Box(Ogre::SceneNode* node, NewtonWorld* world, float mass);
	virtual ~Box();

	void sync();
private:
	NewtonCollision* createBox(NewtonWorld* world);
	NewtonBody* createRigidBody(NewtonWorld* world, NewtonCollision* shape);

	static void setTransformCallback(const NewtonBody* body, const float* matrix, int thread);
	static void applyForceCallback(const NewtonBody* body, dFloat timestep, int thread);

	virtual void setTransform(const float* matrix, int thread);
	virtual void applyForce(dFloat timestep, int thread);

	PhysicsWorld* getWorld();

protected:
	Ogre::SceneNode* mNode;
	NewtonBody* mBody;
	float mMass;
};

class Surface : public Box
{
public:
	Surface(Ogre::SceneNode* node, NewtonWorld* world)
		: Box(node, world, 0.0f)
	{ }
};

class CompanionCube : public Box
{
public:
	CompanionCube(Ogre::SceneNode* node, NewtonWorld* world)
		: Box(node, world, 0.0f)
	{ }
};

#endif

