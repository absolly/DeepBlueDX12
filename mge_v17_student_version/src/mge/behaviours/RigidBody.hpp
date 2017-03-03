#pragma once
#include "mge/core/World.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <btBulletDynamicsCommon.h>

class RigidBody : public AbstractBehaviour, public btRigidBody {
public: 
	RigidBody(Collider& collider, float pMass = 1, btDefaultMotionState* pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))), btVector3& pInertia = btVector3(0, 0, 0));
	RigidBody(Collider& collider, btRigidBody::btRigidBodyConstructionInfo& pConstructionInfo);
	~RigidBody();

	virtual void update(float pStep);
	void updateTransform();

	Collider& getCollider();
protected:
private:
	btVector3 calculateIntertia(const float pMass, const btCollisionShape * pShape, btVector3& pInertia) const;
	Collider& _collider;
};