#pragma once
#include "mge\core\Physics\CollisionBehaviour.h"

class Collider : public CollisionBehaviour
{
public:
	Collider(class btCollisionShape& colliderShape, class GameObject& owner, bool isTrigger, bool isStatic);
	class RigidBody& makeRigidBody(float mass, btVector3 & inertia, btDefaultMotionState & defaultMotionState);
	~Collider();
	class btCollisionShape& getColliderShape();

	void setIsTrigger(const bool trigger);
private:
	class btCollisionShape& _colliderShape;

	bool _isTrigger;
};

struct ColliderArgs
{

};