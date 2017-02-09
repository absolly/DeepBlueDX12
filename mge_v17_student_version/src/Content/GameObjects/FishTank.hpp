#ifndef FISHTANK_H
#define FISHTANK_H

#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>

#include <mge/core/World.hpp>
#include <vector>
#include <mge/core/GameObject.hpp>

class FishTank : public GameObject
{
public:
	FishTank(glm::vec3 pPosition, World * pWorld, std::string pName, int pTankSize, int pFishCount);
	glm::vec3 goalPosition = glm::vec3(15, 10, 0);
	std::vector<GameObject*> * allFish;
	virtual ~FishTank();
	int getTankSize();
	int getFishCount();
	void SetNewGoal();

private:
	std::vector<glm::vec3> * _waypoints;
	int _tankSize = 0;
	int _goalIndex = 0;
	int _fishCount = 0;
	void update(float pStep);
};

#endif // GAMEOBJECT_H
