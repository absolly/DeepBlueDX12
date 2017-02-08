#pragma once
#include "mge/core/World.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <btBulletDynamicsCommon.h>

class RigidBody : public AbstractBehaviour{
public: 
	RigidBody(float pMass = 1, btDefaultMotionState* pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))), btCollisionShape* pShape = new btSphereShape(1), glm::vec3 pInertia = glm::vec3(0, 0, 0));
	RigidBody(btRigidBody::btRigidBodyConstructionInfo& pConstructionInfo);
	~RigidBody();

	virtual void update(float pStep);
	btRigidBody* rigidBody = nullptr;
protected:
	GameObject* _owner;
};