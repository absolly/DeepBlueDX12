#include <cassert>
#include <iostream>
#include <string>
#include <time.h>
#include <random>
using namespace std;

#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"
#include "Content/GameObjects/PlayerFishFlock.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "Content/Behaviours/FlockingBehaviour.hpp"
#include "mge/materials/GPUinstancingMaterial.hpp"

PlayerFishFlock::PlayerFishFlock(glm::vec3 pPosition, World * pWorld, std::string pName, GameObject * pPlayer) : GameObject(pName ,pPosition)
{

	allFish = new vector<GameObject*>();
	_player = pPlayer;
	_parentPos = getLocalPosition();

	Mesh* cubeMeshF = Mesh::load(Config::MGE_MODEL_PATH + "small_fish.OBJ");
	AbstractMaterial* colormat = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"), 1, 10);

	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(1, 40 / 4);

	goalPosition = pPlayer->getWorldPosition();
	std::cout << goalPosition << std::endl;

	for (int i = 0; i < 100; i++)
	{
		int randomX = dis(gen);
		int randomY = dis(gen);
		int randomZ = dis(gen);
		AbstractBehaviour* flock = new FlockingBehaviour(this, allFish, 10, goalPosition, 38, 100, true);

		GameObject* fish = new GameObject("fish", glm::vec3(0, 0, 0));
		fish->addBehaviour(flock);
		allFish->push_back(fish);
		_AllowedToExist = false;
	}
};

void PlayerFishFlock::update(float pStep)
{
	if (_AllowedToExist)
	{
		if (glm::distance(goalPosition, _placedPos) < 40.0f)
			goalPosition = _player->getWorldPosition();

		for (int i = 0; i < allFish->size(); i++)
		{
			allFish->at(i)->update(pStep);
		}
	}
}

void PlayerFishFlock::CallFish()
{
	_placedPos = _player->getWorldPosition();
	glm::vec3 spawnPoint = _player->getTransform() * glm::vec4(0,0,-20,1);
	setLocalPosition(_player->getWorldPosition());

	goalPosition = _player->getWorldPosition();

	
	_AllowedToExist = true;

	for (int i = 0; i < 100; i++)
	{
		allFish->at(i)->setLocalPosition(spawnPoint);
	}
}

void PlayerFishFlock::spawnFishes()
{

}

glm::vec3 PlayerFishFlock::updatePlayerPos()
{
	//goalPosition = 
	return goalPosition;
}

PlayerFishFlock::~PlayerFishFlock()
{
}