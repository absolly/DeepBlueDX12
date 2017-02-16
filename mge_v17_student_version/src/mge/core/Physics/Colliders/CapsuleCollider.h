#pragma once

#include "Collider.h"

struct CapsuleColliderArgs
{
	float radius;
	float height;
	CapsuleColliderArgs(float radius, float height) : radius(radius), height(height) {}
};

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(CapsuleColliderArgs& capsuleColliderArgs, class GameObject& owner, bool isTrigger);
};