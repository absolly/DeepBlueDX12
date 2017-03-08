#include "DivingBehaviour.h"
#include "mge/core/GameObject.hpp"
#include "Content\Core\Input.h"
#include "Content\Hud\Hud.hpp"
#include "mge\core\SoundManager.hpp"

DivingBehaviour::DivingBehaviour(float pTankSize, float pDrainRate, float pRefillCooldown) : _tankSize(pTankSize), _drainRate(pDrainRate), _airLeft(20), _refillCooldown(pRefillCooldown), _refillCooldownTimer(pRefillCooldown)
{
}


DivingBehaviour::~DivingBehaviour()
{
}

void DivingBehaviour::update(float pStep)
{
	float previousAirLeft = _airLeft;
	_airLeft -= _drainRate * 0.5f * pStep;

	if (_airLeft <= 15 && previousAirLeft > 15)
	{
		SoundManager::getInstance()->PlaySound("Careful with the air", "Careful with the air", false, true, false);
	}
	if (_airLeft <= 0 && previousAirLeft > 0)
	{
		SoundManager::getInstance()->PlaySound("suffocating", "suffocating", false, true, false);
	}

	_refillCooldownTimer -= pStep;

	/*if (_owner->getWorldPosition().y >  727.386)
		_airLeft = _tankSize;*/
	if (_timer <= 0) {
		//std::cout << "air left: " << _airLeft << std::endl;
		_timer = 10;
	}

	_timer--;


	Hud::getInstance()->setDepth((int)ceil(71.7 - _owner->getChildAt(0)->getWorldPosition().y / 10));
	Hud::getInstance()->setOxygenLeft(to_string((int)ceil(_airLeft)));
}

void DivingBehaviour::onCollisionAddAir(OnCollisionArgs onCollisionArgs)
{
	int cost = _tankSize - _airLeft;

	if (_refillCooldownTimer <= 0 && cost < Hud::getInstance()->getCoinCount()) {
		std::string str = "Press E to refill air (cost: ";
		str += to_string(cost);
		str += " gold)";
		Hud::getInstance()->setInteractionText(str);
	}
	else if (_refillCooldownTimer <= 0 && cost > Hud::getInstance()->getCoinCount()) {
		std::string str = "Not enough gold to refill air (cost: ";
		str += to_string(cost);
		str += " gold)";
		Hud::getInstance()->setInteractionText(str);
	}
	else {
		Hud::getInstance()->setInteractionText("");
	}

	if (Input::getKeyDown(sf::Keyboard::E) && _refillCooldownTimer <= 0 && cost < Hud::getInstance()->getCoinCount()) {
		std::cout << "added air" << std::endl;
		Hud::getInstance()->addCoin(-cost);
		_airLeft = _tankSize;
		_refillCooldownTimer = _refillCooldown;
		SoundManager::getInstance()->StopSoundChannel("suffocating");
	}
}

void DivingBehaviour::onExitCollisionAddAir(OnCollisionArgs onCollisionArgs)
{
	Hud::getInstance()->setInteractionText("");
	//std::cout << "exit trigger" << std::endl;

}
