#include "Collider.h"
#include "btBulletCollisionCommon.h"

Collider::Collider(btCollisionShape& colliderShape) :
	_colliderShape(colliderShape)
{
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
