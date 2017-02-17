#include "DivingBehaviour.h"
#include "mge/core/GameObject.hpp"


DivingBehaviour::DivingBehaviour(float pTankSize, float pDrainRate) : _tankSize(pTankSize), _drainRate(pDrainRate), _airLeft(pTankSize)
{
}


DivingBehaviour::~DivingBehaviour()
{
}

void DivingBehaviour::update(float pStep)
{
	_airLeft -= _drainRate * pStep;
	_timer--;

	if (_owner->getWorldPosition().y >  727.386)
		_airLeft = _tankSize;
	if (_timer <= 0) {
		std::cout << "air left: " << _airLeft << std::endl;
		_timer = 10;
	}

	


}
