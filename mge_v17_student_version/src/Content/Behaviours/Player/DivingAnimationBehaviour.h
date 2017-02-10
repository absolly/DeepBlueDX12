#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "mge\core\GameObject.hpp"

class DivingAnimationBehaviour : public AbstractBehaviour
{
public:
	DivingAnimationBehaviour();
	virtual void update(float pStep);

private:
	float _currentOffset = 0;
	bool _moveUp = false;
	float _currentMoveUpSpeed = 0;
};