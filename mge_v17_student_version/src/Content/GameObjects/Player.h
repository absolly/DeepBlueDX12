#pragma once

#include "mge\core\GameObject.hpp"
#include "Content\Behaviours\Player\PlayerMovementBehaviour.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

private:
	PlayerMovementBehaviour* _playerMovementBehaviour;
};