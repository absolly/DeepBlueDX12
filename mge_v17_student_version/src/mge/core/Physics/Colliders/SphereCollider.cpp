#include "SphereCollider.h"
#include "btBulletCollisionCommon.h"

SphereCollider::SphereCollider(SphereColliderArgs& sphereColliderArgs) :
	Collider(*new btSphereShape(sphereColliderArgs.radius))
{
}

SphereCollider::~SphereCollider()
{
}
