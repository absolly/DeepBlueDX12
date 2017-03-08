#include <cassert>
#include <iostream>
#include <string>
#include <time.h>
#include <random>
using namespace std;

#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"
#include "Content/GameObjects/FishTank.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "Content/Behaviours/FlockingBehaviour.hpp"
#include "mge/materials//GPUinstancingMaterial.hpp"

FishTank::FishTank(glm::vec3 pPosition, World * pWorld, GameObject * pPlayer, std::string pName, int pTankSize, int pFishCount, int updateRate) : GameObject(pName ,pPosition)
{
	_tankSize = pTankSize;
	_fishCount = pFishCount;
	_player = pPlayer;


	allFish = new vector<GameObject*>();
	
	_parentPos = getLocalPosition();

	//_waypoints->push_back(glm::vec3(pTankSize , pTankSize / 2, -pTankSize));
	//_waypoints->push_back(glm::vec3(pTankSize / 2, pTankSize / 2, pTankSize / 2));
	//_waypoints->push_back(glm::vec3(-pTankSize / 2, -pTankSize / 2, -pTankSize / 2));
	//_waypoints->push_back(glm::vec3(-pTankSize, pTankSize / 2, pTankSize));


	
	Mesh* cubeMeshF = Mesh::load(Config::MGE_MODEL_PATH + "small_fish.OBJ");
	AbstractMaterial* colormat = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"), 1, 10);

	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(1, pTankSize / 4);

	goalPosition = glm::vec3(dis(gen), dis(gen), dis(gen)) + getLocalPosition();


	for (int i = 0; i < _fishCount; i++)
	{
		int randomX = dis(gen);
		int randomY = dis(gen);
		int randomZ = dis(gen);
		AbstractBehaviour* flock = new FlockingBehaviour(this, allFish, updateRate, goalPosition, _tankSize, _fishCount);

		GameObject* fish = new GameObject("fish", glm::vec3(_parentPos.x + randomX, _parentPos.y + randomY, _parentPos.z + randomZ));
		fish->addBehaviour(flock);
		pWorld->add(fish);
		allFish->push_back(fish);
	}

};

int FishTank::getTankSize()
{
	return _tankSize;
}

int FishTank::getFishCount()
{
	return _fishCount;
}

void FishTank::SetRenderDistance(int pDistance)
{
	renderDistance += renderDistance + _tankSize;
}

void FishTank::update(float pStep)
{
	for (GameObject * fish : *allFish)
	{
		fish->update(pStep);
	}

	if (glm::distance(getWorldPosition(), _player->getWorldPosition()) < renderDistance)
	{
		getMaterial()->allowedToRender = true;

		for (GameObject * fish : *allFish)
		{
			fish->update(pStep);
		}
	}
	else
	{
		getMaterial()->allowedToRender = false;
	}
}

std::vector<glm::vec3> * waypointsValidator(std::vector<glm::vec3> * pWaypoints)
{
	if(pWaypoints->size() < 1)
		std::cout << "this waypoint list is empty so the fish will go only go to their starting position";

	//for (size_t i = 0; i < pWaypoints->size(); i++)
	//{
	//	if (glm::distance(pWaypoints->at(i), ))
	//}
	return nullptr;
}

void FishTank::SetNewGoal()
{
	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(1, _tankSize * 1.5);

	int randomX = dis(gen) - _tankSize;
	int randomY = dis(gen) - _tankSize;
	int randomZ = dis(gen) - _tankSize;

	goalPosition = glm::vec3(randomX, randomY, randomZ) + getLocalPosition();

}


FishTank::~FishTank()
{
}