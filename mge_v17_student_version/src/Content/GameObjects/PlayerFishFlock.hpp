#ifndef PLAYERFISHFLOCK_H
#define PLAYERFISHFLOCK_H

#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>

#include <mge/core/World.hpp>
#include <vector>
#include <mge/core/GameObject.hpp>

class PlayerFishFlock : public GameObject
{
public:
	PlayerFishFlock(glm::vec3 pPosition, World * pWorld, std::string pName, GameObject * pPlayer);
	glm::vec3 goalPosition;
	std::vector<GameObject*> * allFish;
	virtual ~PlayerFishFlock();
	glm::vec3 updatePlayerPos();
	void spawnFishes();
	void CallFish();

private:
	glm::vec3 _parentPos;
	glm::vec3 _placedPos;
	GameObject * _player;
	bool _AllowedToExist = true;

	int _tankSize = 50;
	void update(float pStep);
};

#endif // GAMEOBJECT_H
