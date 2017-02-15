#include "RigidBody.hpp"

RigidBody::RigidBody(float pMass, btDefaultMotionState * pMotionState, btCollisionShape * pShape, btVector3& pInertia) :
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

btVector3 RigidBody::calculateIntertia(const float pMass, const btCollisionShape * pShape, btVector3& pInertia) const
{
	pShape->calculateLocalInertia(pMass, pInertia);
	return pInertia;
}

