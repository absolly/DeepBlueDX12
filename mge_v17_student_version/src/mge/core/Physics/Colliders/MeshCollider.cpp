#include "MeshCollider.h"


MeshCollider::MeshCollider(MeshColliderArgs& meshColliderArgs, GameObject& owner, bool isTrigger) :
	Collider(meshColliderArgs.collisionShape, owner, isTrigger)
{
}