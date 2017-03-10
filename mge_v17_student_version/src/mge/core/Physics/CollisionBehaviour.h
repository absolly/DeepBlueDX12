#pragma once

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include <btBulletDynamicsCommon.h>
#include "Content\Core\Event.h"
#include <unordered_map>

struct OnCollisionArgs
{
	btCollisionObject* sender;
	btCollisionObject* collidingWith;
	OnCollisionArgs(btCollisionObject* sender, btCollisionObject* collidingWith) : sender(sender), collidingWith(collidingWith) {}
};

class CollisionBehaviour : public AbstractBehaviour, public btPairCachingGhostObject
{
public:
	//Trigger();
	CollisionBehaviour(GameObject& parent, btCollisionShape* shape, bool isTrigger, bool usePhysicsPosition, bool isStatic);
	~CollisionBehaviour();
	void updateScale();
	void updatePositon();
	void makeDynamic();
	virtual void update(float pStep);
	//class RigidBody & makeRigidBody(float mass, btVector3 & inertia, btDefaultMotionState & defaultMotionState);
	void checkForCollisions();

	void debugDrawContacts();

	std::unordered_map<btCollisionObject*, Event<OnCollisionArgs>> collisionEvents;
	std::unordered_map<btCollisionObject*, Event<OnCollisionArgs>> collisionEnterEvents;
	std::unordered_map<btCollisionObject*, Event<OnCollisionArgs>> collisionExitEvents;
	std::unordered_set<btCollisionObject*> _collidingObjects;


protected:
	class PhysicsWorld* _physicsWorld;
	btCollisionShape* _collisionShape;
	bool _hasRigidbody = false;
	bool _isTrigger = false;
	bool _usePhysicsPosition = false;
	bool _isStatic = false;
};