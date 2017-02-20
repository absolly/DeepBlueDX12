#pragma once

#include "Collider.h"
#include "mge\core\Mesh.hpp"
#include "btBulletDynamicsCommon.h"

struct MeshColliderArgs
{
	btCollisionShape& collisionShape;
	MeshColliderArgs(Mesh& mesh) : collisionShape(*mesh.getMeshCollisionShape()) {}
	MeshColliderArgs(btBvhTriangleMeshShape& bvhTriangleMeshShape) : collisionShape(bvhTriangleMeshShape) {}
};

class MeshCollider : public Collider
{
public:
	MeshCollider(MeshColliderArgs& colliderArgs, class GameObject& owner, bool isTrigger, bool isStatic);
};
