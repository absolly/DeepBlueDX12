#include "DivingBehaviour.h"
#include "mge/core/GameObject.hpp"
#include "Content\Core\Input.h"
#include "Content\Hud\Hud.hpp"

DivingBehaviour::DivingBehaviour(float pTankSize, float pDrainRate, float pRefillCooldown) : _tankSize(pTankSize), _drainRate(pDrainRate), _airLeft(pTankSize), _refillCooldown(pRefillCooldown), _refillCooldownTimer(pRefillCooldown)
{
}


DivingBehaviour::~DivingBehaviour()
{
}

void DivingBehaviour::update(float pStep)
{
	_airLeft -= _drainRate * 0.5f * pStep;
	_refillCooldownTimer -= pStep;
	
	/*if (_owner->getWorldPosition().y >  727.386)
		_airLeft = _tankSize;*/
	if (_timer <= 0) {
		//std::cout << "air left: " << _airLeft << std::endl;
		_timer = 10;
	}

	_timer--;
	
	
	Hud::getInstance()->setDepth((int)ceil(71.7-_owner->getWorldPosition().y/10));
	Hud::getInstance()->setOxygenLeft(to_string((int)ceil(_airLeft)));
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
