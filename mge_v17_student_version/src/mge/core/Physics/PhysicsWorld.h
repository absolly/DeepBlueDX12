#pragma once

#include "DebugDrawer.h"
#include <btBulletDynamicsCommon.h>
#include "Content\Core\EventHandler.h"

class PhysicsWorld : public btDiscreteDynamicsWorld
{
public:
	PhysicsWorld(World* pMainCamera);
	~PhysicsWorld();
	void TogglePhysicsDebug(sf::Event::KeyEvent& event);
	class btDiscreteDynamicsWorld& createDiscreteDynamicsWorld();
	btBroadphaseInterface & createOverlappingPairCache() const;
	btOverlappingPairCache * getOverlappingPairCache() const;
private:
	class btBroadphaseInterface* _overlappingPairCache;
	DebugDrawer debugDrawer;
};