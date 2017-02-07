#include "mge/core/World.hpp"

using namespace std;

set<Light*> World::activeLights = set<Light*>();
btDiscreteDynamicsWorld* World::dynamicsWorld;

World::World():GameObject("root"), _mainCamera(0) {
    //ctor

    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}

void World::addRigidBody(btRigidBody* pBody) {
    World::dynamicsWorld->addRigidBody(pBody);
}

void World::removeRigidBody(btRigidBody* pBody) {
    World::dynamicsWorld->removeRigidBody(pBody);
}

void World::updatePhysics(float pDelta) {
    dynamicsWorld->stepSimulation(pDelta, 10);
}
