#include "mge/core/World.hpp"
#include "mge\core\Physics\PhysicsWorld.h"
#include "mge\core\Physics\Trigger.h"

using namespace std;

set<Light*> World::activeLights = set<Light*>();
PhysicsWorld* World::physics;


std::vector<btVector3> collisions;
void myTickCallback(btDynamicsWorld *dynamicsWorld, btScalar timeStep) {
	collisions.clear();
	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold *contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		// TODO those are unused. What can be done with them?
		// I think they are the same objects as those in the main loop
		// dynamicsWorld->getCollisionObjectArray() and we could compare
		// the pointers to see which object collided with which.
		{
			const btCollisionObject *objA = contactManifold->getBody0();
			const btCollisionObject *objB = contactManifold->getBody1();
		}
		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			const btVector3& ptA = pt.getPositionWorldOnA();
			const btVector3& ptB = pt.getPositionWorldOnB();
			const btVector3& normalOnB = pt.m_normalWorldOnB;
			collisions.push_back(ptA);
			//collisions.push_back(ptB);
			//collisions.push_back(normalOnB);
		}
	}
	std::cout << "Checked for collisions: " << collisions.size() << std::endl;
}


World::World():GameObject("root"), _mainCamera(NULL) {
    //ctor

	physics = new PhysicsWorld(this);
	//Trigger* trigger = new Trigger(*physics);
	//addBehaviour(trigger);
   /* btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setInternalTickCallback(myTickCallback);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));*/
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}

void World::addRigidBody(btRigidBody* pBody) {
    physics->addRigidBody(pBody);
}

void World::removeRigidBody(btRigidBody* pBody) {
    physics->removeRigidBody(pBody);
}

void World::updatePhysics(float pDelta) {
	physics->stepSimulation(pDelta, 10);
}

void World::debugDraw()
{
	physics->debugDrawWorld();
}
