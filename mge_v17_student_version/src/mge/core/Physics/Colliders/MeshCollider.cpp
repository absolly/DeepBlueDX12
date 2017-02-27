#include "MeshCollider.h"


MeshCollider::MeshCollider(MeshColliderArgs& meshColliderArgs, GameObject& owner, bool isTrigger, bool isStatic) :
	Collider(meshColliderArgs.collisionShape, owner, isTrigger, isStatic)
{
}