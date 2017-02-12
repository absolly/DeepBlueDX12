#pragma once

#include "DebugDrawer.h"
#include <btBulletDynamicsCommon.h>
class PhysicsWorld : public btDiscreteDynamicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();
	class btDiscreteDynamicsWorld& createDiscreteDynamicsWorld();
	btBroadphaseInterface & createOverlappingPairCache() const;
	btOverlappingPairCache * getOverlappingPairCache() const;
private:
	class btBroadphaseInterface* _overlappingPairCache;
	DebugDrawer debugDrawer;
};