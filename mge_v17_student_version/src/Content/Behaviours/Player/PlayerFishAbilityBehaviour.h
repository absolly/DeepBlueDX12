#pragma once

#include "mge\behaviours\AbstractBehaviour.hpp"
#include <string>
#include "Content/GameObjects/PlayerFishFlock.hpp"
#include <mge/core/World.hpp>

class Player;

class PlayerFishAbilityBehaviour : public AbstractBehaviour
{
public:
	PlayerFishAbilityBehaviour(World * pWorld, GameObject * pOwner);
	~PlayerFishAbilityBehaviour();

	virtual void update(float deltaTime);

private:
	World * _world;
	PlayerFishFlock * _playerFishFlock;
};
