#include "SphereCollider.h"
#include "btBulletCollisionCommon.h"

SphereCollider::SphereCollider(SphereColliderArgs& sphereColliderArgs, GameObject& owner, bool isTrigger) :
	Collider(*new btSphereShape(sphereColliderArgs.radius), owner, isTrigger)
{
}

SphereCollider::~SphereCollider()
{
}
