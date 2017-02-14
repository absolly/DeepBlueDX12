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

FishTank::FishTank(glm::vec3 pPosition, World * pWorld, std::string pName, int pTankSize, int pFishCount) : GameObject(pName ,pPosition)
{
	_tankSize = pTankSize;
	_fishCount = pFishCount;

	allFish = new vector<GameObject*>();
	_waypoints = new vector<glm::vec3>();

	_waypoints->push_back(glm::vec3(pTankSize, pTankSize / 2, -pTankSize));
	_waypoints->push_back(glm::vec3(pTankSize / 2, pTankSize / 2, pTankSize / 2));
	_waypoints->push_back(glm::vec3(-pTankSize / 2, -pTankSize / 2, -pTankSize / 2));
	_waypoints->push_back(glm::vec3(-pTankSize, pTankSize / 2, pTankSize));


	
	Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "Fish1_OBJ.OBJ");
	AbstractMaterial* colormat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), 1, 10);

	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(1, pTankSize / 4);

	for  (int i = 0; i < _fishCount; i++)
	{
		int randomX = dis(gen);
		int randomY = dis(gen);
		int randomZ = dis(gen);
		AbstractBehaviour* flock = new FlockingBehaviour(this);

		GameObject* fish = new GameObject("fish", glm::vec3(randomX, randomY, randomZ));
		fish->setMaterial(colormat);
		fish->setMesh(cubeMeshF);
		fish->addBehaviour(flock);
		fish->scale(glm::vec3(.01,.01,.01));
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

void FishTank::update(float pStep)
{
	std::cout << "update loop is clearly working" << std::endl;
}

void FishTank::SetNewGoal()
{
	if (_goalIndex > 3)
	{ 
		_goalIndex = 0;
	}

	std::cout << "new Goal" << std::endl;
	std::cout << _goalIndex << std::endl;

	goalPosition = _waypoints->at(_goalIndex);

	_goalIndex++;
}


FishTank::~FishTank()
{
}