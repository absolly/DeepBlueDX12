#pragma once


class Collider
{
public:
	Collider(class btCollisionShape& colliderShape);
	~Collider();
	class btCollisionShape& getColliderShape();
private:
	class btCollisionShape& _colliderShape;
};

struct ColliderArgs
{

};