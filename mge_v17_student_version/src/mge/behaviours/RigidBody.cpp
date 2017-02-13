#include "RigidBody.hpp"

RigidBody::RigidBody(float pMass, btDefaultMotionState * pMotionState, btCollisionShape * pShape, glm::vec3 pInertia) : 
	btRigidBody(btRigidBody::btRigidBodyConstructionInfo(pMass, pMotionState, pShape, calculateIntertia(pMass, pShape, pInertia)))
{
	//rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(pMass, pMotionState, pShape, intertia));
	World::addRigidBody(this);
}

RigidBody::RigidBody(btRigidBody::btRigidBodyConstructionInfo& pConstructionInfo) : 
	btRigidBody(pConstructionInfo)
{
	World::addRigidBody(this);
}

RigidBody::~RigidBody()
{
}

void RigidBody::update(float pStep)
{
}

btVector3 RigidBody::calculateIntertia(const float pMass, const btCollisionShape * pShape, const glm::vec3 pInertia) const
{
	btVector3 intertia(pInertia.x, pInertia.y, pInertia.z);
	pShape->calculateLocalInertia(pMass, intertia);
	return intertia;
}

