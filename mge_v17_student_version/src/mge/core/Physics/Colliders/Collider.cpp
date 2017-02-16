#include "Collider.h"
#include "btBulletCollisionCommon.h"
#include "mge\core\GameObject.hpp"
#include "mge\behaviours\RigidBody.hpp"

Collider::Collider(btCollisionShape& colliderShape, GameObject& owner, bool isTrigger) : 
	CollisionBehaviour(owner, &colliderShape, isTrigger, false),
	_colliderShape(colliderShape)
{
}

RigidBody& Collider::makeRigidBody(float mass, btVector3& inertia, btDefaultMotionState& defaultMotionState)
{
	_colliderShape.calculateLocalInertia(mass, inertia);
	RigidBody& rigidBody = *new RigidBody(*this, mass, &defaultMotionState, inertia);
	_owner->addBehaviour(&rigidBody);
	_usePhysicsPosition = true;
	return rigidBody;
}

Collider::~Collider()
{
	delete &_colliderShape;
}

btCollisionShape & Collider::getColliderShape()
{
	return _colliderShape;
}

void Collider::setIsTrigger(const bool trigger)
{
	_isTrigger = trigger;
}
