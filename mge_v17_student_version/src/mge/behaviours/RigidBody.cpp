#include "RigidBody.hpp"
#include "btBulletCollisionCommon.h"
#include <glm\gtc\type_ptr.hpp>


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
	//btTransform btTransform = getWorldTransform();
	glm::mat4& transform = glm::mat4(1);// = glm::mat4(_owner->getWorldTransform());
	/*glm::vec3 scale = glm::vec3(glm::length(btTransform[0]), glm::length(transform[1]), glm::length(transform[2]));
	transform = glm::scale(transform, glm::vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
	glm::quat rotation = glm::quat_cast(transform);*/
	btTransform& rigidTransform = getWorldTransform();
	btVector3& position = rigidTransform.getOrigin();
	//transform.setIdentity();
	//transform.setOrigin(btVector3(position.x, position.y, position.z));
	//transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
	
	transform[3][0] = position.getX();
	transform[3][1] = position.getY();
	transform[3][2] = position.getZ();
	btQuaternion& rotation = rigidTransform.getRotation();
	
	
	
	glm::quat quat;
	quat.x = rotation.getX();
	quat.y = rotation.getY();
	quat.z = rotation.getZ();
	quat.w = rotation.getW();

	glm::mat4 mat = glm::mat4_cast(quat);

	_owner->setWorldTransform_TEST(transform * mat);
}

btVector3 RigidBody::calculateIntertia(const float pMass, const btCollisionShape * pShape, btVector3& pInertia) const
{
	pShape->calculateLocalInertia(pMass, pInertia);
	return pInertia;
}

