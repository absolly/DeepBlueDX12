#pragma once

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "Content\Core\Event.h"
#include <unordered_map>

class Trigger : public AbstractBehaviour, public btPairCachingGhostObject
{
public:
	//Trigger();
	Trigger(class PhysicsWorld& dynamicsWorld, btCollisionShape* shape);
	~Trigger();
	virtual void update(float pStep);
	void checkForCollisions();

	void debugDrawContacts();

	std::unordered_map<btCollisionObject*, Event<btCollisionObject*>> collisionEvents;
private:
	class PhysicsWorld* _physicsWorld;
	btCollisionShape* _collisionShape;
};