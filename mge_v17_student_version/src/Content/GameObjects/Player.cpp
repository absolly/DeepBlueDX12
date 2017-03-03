#include "Player.h"
#include "mge\materials\TextureMaterial.hpp"
#include "mge\core\Mesh.hpp"
#include "mge\core\Physics\CollisionBehaviour.h"
#include "mge\core\World.hpp"
#include "mge\core\Physics\PhysicsWorld.h"
#include "mge\core\SoundManager.hpp"
#include "mge\core\Time.h"
#include <random>

Player::Player() : GameObject("Player")
{
	Mesh* cubeMeshF = Mesh::load(Config::MGE_MODEL_PATH + "cube_flat.obj");
	setMesh(cubeMeshF);
	AbstractMaterial* material = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"), 1, 10);
	setMaterial(material);
	_playerMovementBehaviour = new PlayerMovementBehaviour(*this);
	addBehaviour(_playerMovementBehaviour);
}

Player::~Player()
{
	
}

void Player::update(float deltaTime)
{
	GameObject::update(deltaTime);
	PlayBreatheSound();
}

void Player::PlayBreatheSound()
{
	if (_breatheDelay > 0)
	{
		_breatheDelay -= Time::DeltaTime;
		return;
	}
	std::random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	std::mt19937 gen(rd());
	
	std::uniform_int_distribution<> dis(1, 6);

	SoundManager* soundManager = SoundManager::getInstance();
	if (_breathingIn)
	{
		soundManager->PlaySound("air_in_" + std::to_string(dis(gen)), "player", false, false, false, 100);
	}
	else
	{
		soundManager->PlaySound("air_out_relaxed_" + std::to_string(dis(gen)), "player", false, false, false, 100);
	}

	std::uniform_real_distribution<> breatheDelayRange(2.0, 3.0);

	_breatheDelay = breatheDelayRange(gen);
	_breathingIn = !_breathingIn;
}
