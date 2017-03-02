#include "DivingBehaviour.h"
#include "mge/core/GameObject.hpp"
#include "Content\Core\Input.h"

DivingBehaviour::DivingBehaviour(float pTankSize, float pDrainRate, float pRefillCooldown) : _tankSize(pTankSize), _drainRate(pDrainRate), _airLeft(pTankSize), _refillCooldown(pRefillCooldown), _refillCooldownTimer(pRefillCooldown)
{
}


DivingBehaviour::~DivingBehaviour()
{
}

void DivingBehaviour::update(float pStep)
{
	_airLeft -= _drainRate * pStep;
	_refillCooldownTimer -= pStep;

	/*if (_owner->getWorldPosition().y >  727.386)
		_airLeft = _tankSize;*/
	if (_timer <= 0) {
		//std::cout << "air left: " << _airLeft << std::endl;
		_timer = 10;
	}

	_timer--;



}

void DivingBehaviour::onCollisionAddAir(OnCollisionArgs onCollisionArgs)
{
	if (_timer <= 0 && _refillCooldownTimer <= 0)
		std::cout << "Press e to add air" << std::endl;

	if (Input::getKeyDown(sf::Keyboard::E) && _refillCooldownTimer <= 0) {
		std::cout << "added air" << std::endl;
		_airLeft = _tankSize;
		_refillCooldownTimer = _refillCooldown;
	}
}
