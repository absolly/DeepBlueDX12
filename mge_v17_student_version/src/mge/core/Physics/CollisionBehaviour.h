#pragma once

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "Content\Core\Event.h"
#include <unordered_map>

class CollisionBehaviour : public AbstractBehaviour, public btPairCachingGhostObject
{
public:
	//Trigger();
	CollisionBehaviour(GameObject& parent, btCollisionShape* shape, bool isTrigger, bool usePhysicsPosition);
	~CollisionBehaviour();
	void updateScale();
	void updatePositon();
	virtual void update(float pStep);
	void checkForCollisions();

	void debugDrawContacts();

	std::unordered_map<btCollisionObject*, Event<btCollisionObject*>> collisionEvents;
private:
	class PhysicsWorld* _physicsWorld;
	btCollisionShape* _collisionShape;
	bool _isTrigger;
	bool _usePhysicsPosition;
};