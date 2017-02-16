#pragma once
#include "Collider.h"
#include "btBulletCollisionCommon.h"

struct BoxColliderArgs : ColliderArgs
{
	btVector3 boxHalfExtends;

	BoxColliderArgs(float width, float height, float depth) : boxHalfExtends(btVector3(width, height, depth)) {}
	BoxColliderArgs(btVector3& boxHalfExtends) : boxHalfExtends(boxHalfExtends) {}
};

class BoxCollider : public Collider
{
public:
	BoxCollider(BoxColliderArgs& boxColliderArgs, class GameObject& owner, bool isTrigger);
};
