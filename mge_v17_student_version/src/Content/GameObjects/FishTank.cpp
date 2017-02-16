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

FishTank::FishTank(glm::vec3 pPosition, World * pWorld, std::string pName, int pTankSize, int pFishCount) : GameObject(pName ,pPosition)
{
	_tankSize = pTankSize;
	_fishCount = pFishCount;

	allFish = new vector<GameObject*>();
	
	_parentPos = getLocalPosition();

	//_waypoints->push_back(glm::vec3(pTankSize , pTankSize / 2, -pTankSize));
	//_waypoints->push_back(glm::vec3(pTankSize / 2, pTankSize / 2, pTankSize / 2));
	//_waypoints->push_back(glm::vec3(-pTankSize / 2, -pTankSize / 2, -pTankSize / 2));
	//_waypoints->push_back(glm::vec3(-pTankSize, pTankSize / 2, pTankSize));


	
	Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "small_fish.OBJ");
	AbstractMaterial* colormat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), 1, 10);

	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(1, pTankSize / 4);

	for (int i = 0; i < _fishCount; i++)
	{
		int randomX = dis(gen);
		int randomY = dis(gen);
		int randomZ = dis(gen);
		AbstractBehaviour* flock = new FlockingBehaviour(this, glm::vec3(1.0f, 1.0f, 1.0f));

		GameObject* fish = new GameObject("fish", glm::vec3(_parentPos.x + randomX, _parentPos.y + randomY, _parentPos.z + randomZ));
		fish->addBehaviour(flock);
		pWorld->add(fish);
		allFish->push_back(fish);
	}

	goalPosition = glm::vec3(dis(gen), dis(gen), dis(gen)) + getLocalPosition();
	

};

int FishTank::getTankSize()
{
	return _tankSize;
}

int FishTank::getFishCount()
{
	return _fishCount;
}

void FishTank::update(float pStep)
{
	
}

std::vector<glm::vec3> * waypointsValidator(std::vector<glm::vec3> * pWaypoints)
{
	if(pWaypoints->size() < 1)
		std::cout << "this waypoint list is empty so the fish will go only go to their starting position";

	//for (size_t i = 0; i < pWaypoints->size(); i++)
	//{
	//	if (glm::distance(pWaypoints->at(i), ))
	//}
}

void FishTank::SetNewGoal()
{
	std::cout << "set new goal" << std::endl;

	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(1, _tankSize * 1.5);

	int randomX = dis(gen);
	int randomY = dis(gen);
	int randomZ = dis(gen);

	goalPosition = glm::vec3(randomX, randomY, randomZ) + getLocalPosition();

}


FishTank::~FishTank()
{
}