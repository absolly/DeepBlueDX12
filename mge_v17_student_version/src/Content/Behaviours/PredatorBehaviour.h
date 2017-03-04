#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "glm.hpp"
#include <vector>
#include <btBulletDynamicsCommon.h>
#include <vector>

class PredatorBehaviour : public AbstractBehaviour {
public:
	PredatorBehaviour(GameObject* pTarget, std::vector<glm::vec3> pWaypoints, GameObject* pWorld);
	~PredatorBehaviour();
	virtual void update(float pStep);

private:
	GameObject* _target;
	glm::vec3 _targetPos;
	std::vector<glm::vec3>& _waypoints;
	int _currentWaypoint = 0;
	glm::vec3 _crumbs[16];
	GameObject* _crumbObjects[16];
	int crumbHead = 0;
	int _crumbCooldown = 0;
	void InterPolateDirection(glm::vec3 pDirection);
};

