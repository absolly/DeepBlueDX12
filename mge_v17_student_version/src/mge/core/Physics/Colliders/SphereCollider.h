#pragma once
#include "Collider.h"

struct SphereColliderArgs : ColliderArgs
{
	float radius;

	SphereColliderArgs(float radius) : radius(radius) {}
};

class SphereCollider : public Collider
{
public:
	SphereCollider(SphereColliderArgs& sphereColliderArgs, class GameObject& owner, bool isTrigger, bool isStatic);
	~SphereCollider();
private:
}; 
