#include "CollisionBehaviour.h"
#include "PhysicsWorld.h"
#include "glm.hpp"
#include "mge\core\GameObject.hpp"
#include "mge\core\World.hpp"

/*Trigger::Trigger() : 
	_ghostObject(*new btPairCachingGhostObject()),
	_dynamicsWorld(nullptr)
{
}*/

CollisionBehaviour::CollisionBehaviour(GameObject& owner, btCollisionShape* shape, bool isTrigger, bool usePhysicsPosition) : AbstractBehaviour(), btPairCachingGhostObject(),
	_physicsWorld(World::physics)
{
	_collisionShape = shape;
	setCollisionShape(_collisionShape);
	_isTrigger = isTrigger;
	_usePhysicsPosition = usePhysicsPosition;
	if (isTrigger)
	{
		setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	_physicsWorld->addCollisionObject(this, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter);
	_owner = &owner;
	updatePositon();
	updateScale();
}

CollisionBehaviour::~CollisionBehaviour()
{
}

void CollisionBehaviour::updateScale()
{
	_collisionShape->setLocalScaling(_owner->getBulletPhysicsLocalScale());
}
void CollisionBehaviour::updatePositon()
{
	setWorldTransform(_owner->getBulletPhysicsTransform());
}
void CollisionBehaviour::update(float pStep)
{
	if (!_usePhysicsPosition)
	{
		updateScale();
		updatePositon();
	}
	if (_isTrigger)
	{
		checkForCollisions();
	}
}

void CollisionBehaviour::checkForCollisions()
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
					
					btCollisionObject* obA = const_cast<btCollisionObject*>(manifold->getBody0());
					btCollisionObject* obB = const_cast<btCollisionObject*>(manifold->getBody1());
					collisionEvents[obB](obB);
					// handle collisions here
				}
			}
		}
	}
}

void CollisionBehaviour::debugDrawContacts()
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