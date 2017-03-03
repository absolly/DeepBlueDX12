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
private:

	void PlayBreatheSound();

	PlayerMovementBehaviour* _playerMovementBehaviour;

	bool _breathingIn = true;
	float _breatheDelay = 0;

	float _depth;
	float _airRemaining;
	float _affraidness;

	std::string breatheInSounds = "breathe";
};