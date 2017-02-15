#pragma once

#include "mge\core\GameObject.hpp"
#include "Content\Behaviours\Player\PlayerMovementBehaviour.h"
#include "Content\Core\EventHandler.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void onSpacePressedEvent(sf::Event::KeyEvent & keyEvent);
	Event<float>& coolFloatEvent;

private:
	PlayerMovementBehaviour* _playerMovementBehaviour;
};