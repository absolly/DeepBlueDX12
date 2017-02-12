#pragma once

#include "mge\core\GameObject.hpp"

class Trigger : public GameObject
{
public:
	//Trigger();
	Trigger(class PhysicsWorld& dynamicsWorld);
	~Trigger();
	virtual void update(float pStep);
	void checkForCollisions();

	void debugDrawContacts();

private:
	class PhysicsWorld* _physicsWorld;
	class btPairCachingGhostObject& _ghostObject;
};