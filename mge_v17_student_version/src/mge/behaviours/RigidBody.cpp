#include "RigidBody.hpp"

RigidBody::RigidBody(float pMass, btDefaultMotionState * pMotionState, btCollisionShape * pShape, glm::vec3 pInertia)
{
	btVector3 intertia(pInertia.x, pInertia.y, pInertia.z);
	pShape->calculateLocalInertia(pMass, intertia);
	rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(pMass, pMotionState, pShape, intertia));
	World::addRigidBody(rigidBody);
}

RigidBody::RigidBody(btRigidBody::btRigidBodyConstructionInfo& pConstructionInfo)
{
	rigidBody = new btRigidBody(pConstructionInfo);
	World::addRigidBody(rigidBody);
}

RigidBody::~RigidBody()
{
}

void RigidBody::update(float pStep)
{
}

