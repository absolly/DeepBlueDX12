#include "PredatorBehaviour.h"
#include "mge/core/GameObject.hpp"


PredatorBehaviour::PredatorBehaviour(GameObject* pTarget, glm::vec3 pWaypoints[10]) : _target(pTarget)
{
	for (int i = 0; i < 10; i++)
	{
		_waypoints[i] = pWaypoints[i];
	}
}

PredatorBehaviour::~PredatorBehaviour()
{
}

void PredatorBehaviour::update(float pStep)
{
	if (glm::distance(_owner->getWorldPosition(), _target->getWorldPosition()) < 30) {
		//std::cout << "following player" << std::endl;
		_targetPos = _target->getWorldPosition();
	} else if(glm::distance(_owner->getWorldPosition(), _waypoints[_currentWaypoint]) > 1){
		//std::cout << "navigating to waypoint " << _currentWaypoint << " current distance: " << glm::distance(_owner->getWorldPosition(), _waypoints[_currentWaypoint]) << std::endl;
		_targetPos = _waypoints[_currentWaypoint];
	}
	else {
		//std::cout << "navigating to next waypoint " << _currentWaypoint << std::endl;
		_currentWaypoint++;
		_currentWaypoint = _currentWaypoint % 10;
		_targetPos = _waypoints[_currentWaypoint];
	}
	InterPolateDirection(_owner->getWorldPosition() - _targetPos);
	_owner->translate(glm::vec3(0, 0, 0.1));
}


void PredatorBehaviour::InterPolateDirection(glm::vec3 pDirection)
{
	glm::vec3 LocalPos = _owner->getLocalPosition();

	glm::quat currentDir = glm::quat_cast(_owner->getTransform());

	//glm::vec3 OriginDirection = _owner->getLocalPosition() * currentDir;

	//glm::vec3 newDirection = glm::normalize(OriginDirection) + glm::normalize((direction * 0.002f));

	//std::cout << currentDir << std::endl;

	//glm::mat4 newMat = glm::eulerAngleXYZ(direction.x, direction.y, direction.z);
	glm::mat4 newMat = glm::inverse(glm::lookAt(LocalPos, LocalPos + pDirection, glm::vec3(0, 1, 0)));
	glm::quat newDir = glm::quat_cast(newMat);

	//std::cout << _owner->getLocalPosition() << std::endl;

	glm::mat4 RotationMatrix = glm::mat4_cast(glm::slerp(currentDir, newDir, 0.02f));

	_owner->setTransform(RotationMatrix);
	_owner->setLocalPosition(LocalPos);
}