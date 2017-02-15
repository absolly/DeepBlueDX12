#include "MeshCollider.h"


MeshCollider::MeshCollider(MeshColliderArgs& meshColliderArgs) :
	Collider(meshColliderArgs.collisionShape)
{
}