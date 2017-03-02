#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include <iostream>
class DivingBehaviour : public AbstractBehaviour {
public:
	DivingBehaviour(float pTankSize = 100, float pDrainRate = 1, float pRefillCooldown = 1);
	~DivingBehaviour();
	virtual void update(float pStep);
	float _tankSize;
	float _drainRate;
	float _airLeft;
	float _refillCooldown;
	float _refillCooldownTimer;
	int _timer = 0;
	void onCollisionAddAir(struct OnCollisionArgs onCollisionArgs);

};

