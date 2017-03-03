#include "CollisionBehaviour.h"
#include "PhysicsWorld.h"
#include "glm.hpp"
#include "mge\core\GameObject.hpp"
#include "mge\core\World.hpp"
#include "mge\core\Physics\RigidBody.hpp"

/*Trigger::Trigger() : 
	_ghostObject(*new btPairCachingGhostObject()),
	_dynamicsWorld(nullptr)
{
}*/

#define BIT(x) (1<<(x))
enum collisiontypes {
	COL_NOTHING = 0, //<Collide with nothing
	COL_DYNAMIC = BIT(0), //<Collide with ships
	COL_STATIC = BIT(1), //<Collide with walls
};


CollisionBehaviour::CollisionBehaviour(GameObject& owner, btCollisionShape* shape, bool isTrigger, bool usePhysicsPosition, bool isStatic) : AbstractBehaviour(), btPairCachingGhostObject(),
	_physicsWorld(World::physics)
{
	_collisionShape = shape;
	setCollisionShape(_collisionShape);
	_isTrigger = isTrigger;
	_isStatic = isStatic;
	_usePhysicsPosition = usePhysicsPosition;
	if (isTrigger)
	{
		setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	
	_physicsWorld->addCollisionObject(this, isStatic ? COL_STATIC : COL_DYNAMIC, isStatic ? COL_DYNAMIC : (COL_STATIC | COL_DYNAMIC));
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
	if (!_isStatic && !_hasRigidbody)
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
	std::unordered_set<btCollisionObject*> thisCheckCollidingObjects;
	std::vector<OnCollisionArgs> collisionEnterEventsToExecute;
	std::vector<OnCollisionArgs> collisionEventsToExecute;
	std::vector<OnCollisionArgs> collisionExitEventsToExecute;

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

					btCollisionObject* collidingWith = isFirstBody ? obB : obA;
					btCollisionObject* self = isFirstBody ? obA : obB;
					//Collision Enter
					if (_collidingObjects.find(collidingWith) == _collidingObjects.end())
					{
						_collidingObjects.insert(collidingWith);
						if (collisionEnterEvents.find(collidingWith) != collisionEnterEvents.end())
							collisionEnterEventsToExecute.push_back(OnCollisionArgs(self, collidingWith));
					}
					//Collision Continuous
					thisCheckCollidingObjects.insert(collidingWith);
					if (collisionEvents.find(collidingWith) != collisionEvents.end())
						collisionEventsToExecute.push_back(OnCollisionArgs(self, collidingWith));
					//std::cout << "COLLISION" << std::endl;
					// handle collisions here
				}
			}
		}
	}
	//Collision Exit
	for (std::unordered_set<btCollisionObject*>::iterator obj = _collidingObjects.begin(); obj != _collidingObjects.end(); ++obj)
	{
		if (thisCheckCollidingObjects.find(*obj) == thisCheckCollidingObjects.end())
		{
			collisionExitEventsToExecute.push_back(OnCollisionArgs(this, *obj));
			obj = _collidingObjects.erase(obj);
		}
	}

	for each (OnCollisionArgs onCollisionArgs in collisionEnterEventsToExecute)
	{
		std::cout << "COLLISION ENTER" << onCollisionArgs.collidingWith << std::endl;
		if (collisionEnterEvents.find(onCollisionArgs.collidingWith) != collisionEnterEvents.end())
			collisionEnterEvents[onCollisionArgs.collidingWith](OnCollisionArgs(onCollisionArgs.sender, onCollisionArgs.collidingWith));
	}
	for each (OnCollisionArgs onCollisionArgs in collisionEventsToExecute)
	{
		if (collisionEvents.find(onCollisionArgs.collidingWith) != collisionEvents.end())
			collisionEvents[onCollisionArgs.collidingWith](OnCollisionArgs(onCollisionArgs.sender, onCollisionArgs.collidingWith));
	}
	for each (OnCollisionArgs onCollisionArgs in collisionExitEventsToExecute)
	{
		std::cout << "COLLISION EXIT" << onCollisionArgs.collidingWith << std::endl;
		if (collisionExitEvents.find(onCollisionArgs.collidingWith) != collisionExitEvents.end())
			collisionExitEvents[onCollisionArgs.collidingWith](OnCollisionArgs(onCollisionArgs.sender, onCollisionArgs.collidingWith));
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