#include "Trigger.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "PhysicsWorld.h"

/*Trigger::Trigger() : 
	_ghostObject(*new btPairCachingGhostObject()),
	_dynamicsWorld(nullptr)
{
}*/

Trigger::Trigger(PhysicsWorld& physicsWorld) :
	_ghostObject(*new btPairCachingGhostObject()),
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
	scale.x = 100;
	scale.y = 100;
	scale.z = 100;
	btConvexShape* box = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
	_ghostObject.setCollisionShape(box);
	_ghostObject.setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
}

Trigger::~Trigger()
{
}

void Trigger::update(float pStep)
{
	checkForCollisions();
	debugDrawContacts();
}

void Trigger::checkForCollisions()
{
	btManifoldArray manifoldArray;
	btBroadphasePairArray& pairArray = _ghostObject.getOverlappingPairCache()->getOverlappingPairArray();
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

			bool isFirstBody = manifold->getBody0() == &_ghostObject;

			btScalar direction = isFirstBody ? btScalar(-1.0) : btScalar(1.0);

			for (int p = 0; p < manifold->getNumContacts(); ++p)
			{
				const btManifoldPoint& pt = manifold->getContactPoint(p);

				if (pt.getDistance() < 0.f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;

					std::cout << "COLLISION!!!!" << std::endl;
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
		btBroadphasePairArray& pairArray = _ghostObject.getOverlappingPairCache()->getOverlappingPairArray();
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