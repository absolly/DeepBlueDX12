#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include <iostream>
#include "mge/core/GameObject.hpp"


class BoatFollowBehaviour : public AbstractBehaviour {
public:
	BoatFollowBehaviour(GameObject * pTarget);
	~BoatFollowBehaviour();
	virtual void update(float pStep);
	
	int _timer = 0;

private:
	void InterPolateDirection(glm::vec3 pDirection, float pSpeed);
	float clip(float n, float lower, float upper);
	GameObject* _target;
};

