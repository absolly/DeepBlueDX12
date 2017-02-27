#include "SphereCollider.h"
#include "btBulletCollisionCommon.h"

SphereCollider::SphereCollider(SphereColliderArgs& sphereColliderArgs, GameObject& owner, bool isTrigger, bool isStatic) :
	Collider(*new btSphereShape(sphereColliderArgs.radius), owner, isTrigger, isStatic)
{
}

SphereCollider::~SphereCollider()
{
}
