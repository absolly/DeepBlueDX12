#pragma once

#include "mge\core\GameObject.hpp"
#include "Content\Behaviours\Player\PlayerMovementBehaviour.h"
#include "Content\Core\EventHandler.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	virtual void update(float deltaTime);
	
	void setAffraidness(float affraidness);
	float getAffraidness();
	void scare(float affraidnessIncrease);

	Camera* getCamera();
private:
	void resetLevel();
	glm::vec3 _spawnPosition;
	Camera* _camera;
	void respawn();
	int getDepthInCM();

	float _depth = 0;
	float _airRemaining = 100;
	float _affraidness = 0;
	int _deaths = 0;
	std::string breatheInSounds = "breathe";
};