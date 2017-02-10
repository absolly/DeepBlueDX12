#include "DivingAnimationBehaviour.h"

#include "glm.hpp"

DivingAnimationBehaviour::DivingAnimationBehaviour()
{
}

void DivingAnimationBehaviour::update(float deltaTime)
{
	float moveRange = 0.1f;

	//Moving up
	//float moveUpSpeed = 1.2f;
	//Moving down
	float moveDownSpeed = 0.15f;

	if (_moveUp)
	{
		_currentMoveUpSpeed += 1.5f * deltaTime;
		_currentOffset += _currentMoveUpSpeed * deltaTime;
		if (_currentOffset > moveRange / 2)
		{
			_moveUp = false;
			_currentMoveUpSpeed /= 2;
			_currentOffset = moveRange / 2;
		}
	}
	else
	{
		_currentMoveUpSpeed -= 0.3f  * deltaTime;
		_currentOffset += _currentMoveUpSpeed * deltaTime;
		if (_currentOffset < -moveRange / 2)
		{
			_moveUp = true;
			_currentMoveUpSpeed /= 2;
			_currentOffset = -moveRange / 2;
		}
	}

	//_currentOffset = glm::clamp(_currentOffset, -moveRange / 2, moveRange / 2);
	glm::mat4 rotationMatrix = glm::extractMatrixRotation(_owner->getWorldTransform());
	
	glm::vec3 ownerPosition = _owner->getLocalPosition();
	ownerPosition.y = _currentOffset;
	_owner->setLocalPosition(ownerPosition);
	
}
