#include <cassert>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"
#include "Content/GameObjects/FishTank.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "Content/Behaviours/FlockingBehaviour.hpp"

FishTank::FishTank(glm::vec3 pPosition, World * pWorld, std::string pName, int pTankSize, int pFishCount) : GameObject(pName ,pPosition)
{
	_tankSize = pTankSize;
	_fishCount = pFishCount;

	allFish = new vector<GameObject*>();

	srand(time(NULL));
	Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	AbstractMaterial* colormat = new ColorMaterial(glm::vec3(0,1,0));

	for  (int i = 0; i < _fishCount; i++)
	{
		int randomX = rand() % pTankSize + 1;
		int randomY = rand() % pTankSize + 1;
		int randomZ = rand() % pTankSize + 1;
		AbstractBehaviour* flock = new FlockingBehaviour(this);

		std::cout << "hi" << std::endl;
		GameObject* fish = new GameObject("fish", glm::vec3(i, i, i));
		fish->setMaterial(colormat);
		fish->setMesh(cubeMeshF);
		fish->scale(glm::vec3(0.2f, 0.2f, 0.2f));
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


FishTank::~FishTank()
{
}