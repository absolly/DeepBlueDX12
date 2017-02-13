#include "Trigger.h"
#include "PhysicsWorld.h"
#include "glm.hpp"
#include "mge\core\GameObject.hpp"
#include "mge\core\World.hpp"

/*Trigger::Trigger() : 
	_ghostObject(*new btPairCachingGhostObject()),
	_dynamicsWorld(nullptr)
{
}*/

Trigger::Trigger(PhysicsWorld& physicsWorld, btCollisionShape* shape) : AbstractBehaviour(), btPairCachingGhostObject(),
	_physicsWorld(&physicsWorld)
{
	btTransform startTransform;
	//startTransform.setIdentity();
	glm::mat4 transform = glm::mat4(1);//getTransform();
	glm::quat rotation = glm::quat_cast(transform);
	glm::vec3 position = transform[3];
	glm::vec3 scale = glm::vec3(glm::length(transform[0]), glm::length(transform[1]), glm::length(transform[2]));
	startTransform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
	
	//startTransform.setOrigin(btVector3(position.x, position.y, position.z));
	scale.x = 1;
	scale.y = 1;
	scale.z = 1;
	//btCollisionShape* shape2 = shape;
	setCollisionShape(shape);
	shape->setLocalScaling(btVector3(4, 4, 4));
	//setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	_physicsWorld->addCollisionObject(this, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter);
}

Trigger::~Trigger()
{
}

void Trigger::update(float pStep)
{
	checkForCollisions();
	setWorldTransform(_owner->getBulletPhysicsTransform());
	debugDrawContacts();
}

void Trigger::checkForCollisions()
{
	btManifoldArray manifoldArray;
	btBroadphasePairArray& pairArray = getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();

	for (int i = 0; i < numPairs; ++i)
	{
		manifoldArray.clear();
		
		const btBroadphasePair& pair = pairArray[i];

		btBroadphasePair* collisionPair = _physicsWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);

		if (!collisionPair) continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		for (int j = 0; j < manifoldArray.size(); j++)
		{
			btPersistentManifold* manifold = manifoldArray[j];
			
			bool isFirstBody = manifold->getBody0() == this;

			btScalar direction = isFirstBody ? btScalar(-1.0) : btScalar(1.0);

			for (int p = 0; p < manifold->getNumContacts(); ++p)
			{
				const btManifoldPoint& pt = manifold->getContactPoint(p);

				if (pt.getDistance() < 0.f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;
					
					//std::cout << "COLLISION!!!!" << rand() << std::endl;
					btCollisionObject* obA = const_cast<btCollisionObject*>(manifold->getBody0());
					btCollisionObject* obB = const_cast<btCollisionObject*>(manifold->getBody1());
					collisionEvents[obB](obB);
					// handle collisions here
				}
			}
		}
	}

}

void Trigger::debugDrawContacts()
{
	//	printf("numPairs = %d\n",m_customPairCallback->getOverlappingPairArray().size());
	{
		btManifoldArray	manifoldArray;
		btBroadphasePairArray& pairArray = getOverlappingPairCache()->getOverlappingPairArray();
		int numPairs = pairArray.size();

		for (int i = 0; i<numPairs; i++)
		{
			manifoldArray.clear();

			const btBroadphasePair& pair = pairArray[i];

			btBroadphasePair* collisionPair = _physicsWorld->getOverlappingPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
			if (!collisionPair)
				continue;

			if (collisionPair->m_algorithm)
				collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

			for (int j = 0; j<manifoldArray.size(); j++)
			{
				btPersistentManifold* manifold = manifoldArray[j];
				for (int p = 0; p<manifold->getNumContacts(); p++)
				{
					const btManifoldPoint&pt = manifold->getContactPoint(p);

					btVector3 color(255, 255, 255);
					_physicsWorld->getDebugDrawer()->drawContactPoint(pt.getPositionWorldOnB(), pt.m_normalWorldOnB, pt.getDistance(), pt.getLifeTime(), color);
				}
			}
		}
	}

}