#include "Player.h"
#include "mge\materials\TextureMaterial.hpp"
#include "mge\core\Mesh.hpp"
#include "mge\core\Physics\CollisionBehaviour.h"
#include "mge\core\World.hpp"
#include "mge\core\Physics\PhysicsWorld.h"
#include "mge\core\SoundManager.hpp"
#include "mge\core\Time.h"
#include "mge\core\Random.h"
#include "Content\Core\Input.h"
#include "mge\core\Camera.hpp"
#include "mge\core\Physics\RigidBody.hpp"
#include "Content\Behaviours\DivingBehaviour.h"
#include "Content\Behaviours\Player\PlayerBreathingBehaviour.h"
#include "Content\Behaviours\Player\DivingAnimationBehaviour.h"
#include "Content/Hud/Hud.hpp"
#include "mge/core/LuaScriptParser.hpp"
#include "Content/TestScene.hpp"

Player::Player() : GameObject("Player")
{
	//Add a camera
	_camera = new Camera("Player Camera", glm::vec3(0, 0, 0), glm::perspective(glm::radians(80.0f), (16.0f / 9.0f), .5f, 100000.0f));
	_camera->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	//Add a camera


	GameObject* temp = new GameObject("PlayerChild0");
	add(temp);
	temp->add(_camera);
	//Add behaviours
	addBehaviour(new PlayerBreathingBehaviour(*this));
	addBehaviour(new DivingAnimationBehaviour());
	addBehaviour(new DivingBehaviour());
	//Add behaviours

	//Set the position
	_spawnPosition = glm::vec3(0,2,0);
	temp->setLocalPosition(_spawnPosition);
	//Set the position


	//Add rigidbody and collider
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(temp->getBulletPhysicsTransform());
	temp->addCollider(SphereColliderArgs(3), false, false).makeRigidBody(1, btVector3(), *fallMotionState);
	temp->getBehaviour<RigidBody>()->setGravity(btVector3(0, 0, 0));
	temp->addBehaviour(new PlayerMovementBehaviour(*this));
	//Add rigidbody and collider

}

Player::~Player()
{
	
}

void Player::update(float deltaTime)
{
	GameObject::update(deltaTime);
	if (Input::getKeyDown(sf::Keyboard::F4) || (Hud::getInstance()->isPlayerKilled && Input::getKeyDown(sf::Keyboard::E)))
		respawn();
	
	if (Input::getKeyDown(sf::Keyboard::F6))
		resetLevel();
}

void Player::setAffraidness(float affraidness)
{
	_affraidness = glm::clamp(affraidness, 0.0f, 100.0f);
}

float Player::getAffraidness()
{
	return _affraidness;
}

void Player::scare(float scareAmount)
{
	_affraidness = glm::clamp(_affraidness + scareAmount, 0.0f, 100.0f);
}

void Player::setSpawnBoat(GameObject * pBoat)
{
	_spawnBoat = pBoat;
}

Camera * Player::getCamera()
{
	return _camera;
}

void Player::resetLevel()
{
	respawn(true);
	Hud::getInstance()->setCoinCount(0);
	TestScene::resetEvent();
}

void Player::respawn(bool reset)
{
	_deaths++;
	GameObject* temp = getChildAt(0);
	if(_spawnBoat && !reset)
		temp->setLocalPosition(_spawnBoat->getWorldPosition() - glm::vec3(0,15,0));
	else
		temp->setLocalPosition(_spawnPosition);
	temp->getBehaviour<RigidBody>()->setWorldTransform(temp->getBulletPhysicsTransform());
	Hud::getInstance()->isPlayerKilled = false;
	getBehaviour<DivingBehaviour>()->_airLeft = 100;
	SoundManager::getInstance()->StopSoundChannel("suffocating");
}

int Player::getDepthInCM()
{
	return (int)ceil(7170 - getWorldPosition().y * 10);
}
