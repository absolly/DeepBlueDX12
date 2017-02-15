#pragma once


class Collider
{
public:
	Collider(class btCollisionShape& colliderShape);
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