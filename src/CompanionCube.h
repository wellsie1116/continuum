
#ifndef COMPANIONCUBE_H_30UP6IUJ
#define COMPANIONCUBE_H_30UP6IUJ

#include <OgreSceneNode.h>

#include <Newton.h>

class CompanionCube {
public:
	CompanionCube(Ogre::SceneNode* node, NewtonWorld* world);
	virtual ~CompanionCube();

	void sync();
private:
	NewtonCollision* createBox(NewtonWorld* world);
	NewtonBody* createRigidBody(NewtonWorld* world, NewtonCollision* shape);

	static void setTransformCallback(const NewtonBody* body, const float* matrix, int thread);
	static void applyForceCallback(const NewtonBody* body, dFloat timestep, int thread);

	void setTransform(const float* matrix, int thread);
	void applyForce(dFloat timestep, int thread);

private:
	Ogre::SceneNode* mNode;
	NewtonBody* mBody;
	
};

#endif
