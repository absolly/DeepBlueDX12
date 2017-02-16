#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "glm.hpp"
#include <vector>

class PredatorBehaviour : public AbstractBehaviour {
public:
	PredatorBehaviour(GameObject* pTarget, glm::vec3 pWaypoints[10]);
	~PredatorBehaviour();
	virtual void update(float pStep);

private:
	GameObject* _target;
	glm::vec3 _targetPos;
	glm::vec3 _waypoints[10]{};
	int _currentWaypoint = 0;
	glm::vec3 _crumbs[64];
	int _crumbCooldown;
	void InterPolateDirection(glm::vec3 pDirection);
};

