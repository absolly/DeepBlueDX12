#include "PhysicsWorld.h"
#include <vector>
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include <iostream>

PhysicsWorld::PhysicsWorld(World* pMainCamera) :
	_overlappingPairCache(&createOverlappingPairCache()),
	btDiscreteDynamicsWorld(createDiscreteDynamicsWorld())
{
	/*debugDrawer.setDebugMode(btIDebugDraw::DBG_NoDebug);
	setDebugDrawer(&debugDrawer); TODO
	debugDrawer._world = pMainCamera;*/
	EventHandler::bindKeyDownEvent(sf::Keyboard::F2, this, &PhysicsWorld::TogglePhysicsDebug);
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::TogglePhysicsDebug(sf::Event::KeyEvent& event)
{
	int debugMode = debugDrawer.getDebugMode();
	debugMode = debugMode == btIDebugDraw::DBG_NoDebug ? btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE : btIDebugDraw::DBG_NoDebug;
	debugDrawer.setDebugMode(debugMode);
}

btDiscreteDynamicsWorld& PhysicsWorld::createDiscreteDynamicsWorld()
{
	//btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	_overlappingPairCache = &createOverlappingPairCache();
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, _overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
	return *dynamicsWorld;
	/*btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	//dynamicsWorld->setInternalTickCallback(myTickCallback);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	return *dynamicsWorld;*/
}

btBroadphaseInterface& PhysicsWorld::createOverlappingPairCache() const
{
	btVector3 worldMin(-1000, -1000, -1000);
	btVector3 worldMax(1000, 1000, 1000);
	btAxisSweep3* sweepBP = new btAxisSweep3(worldMin, worldMax);
	sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	return *sweepBP;
}

btOverlappingPairCache * PhysicsWorld::getOverlappingPairCache() const
{
	return _overlappingPairCache->getOverlappingPairCache();
}
