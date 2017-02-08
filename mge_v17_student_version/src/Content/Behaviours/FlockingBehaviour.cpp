#include "Content/Behaviours/FlockingBehaviour.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "Content/GameObjects/FishTank.hpp"


FlockingBehaviour::FlockingBehaviour(FishTank * pFishtank) : AbstractBehaviour()
{
	fishtank = pFishtank;
}

FlockingBehaviour::~FlockingBehaviour()
{

}

void FlockingBehaviour::update(float pStep)
{
	if (glm::distance(_owner->getWorldPosition(), glm::vec3(0, 0, 0)) >= (fishtank->getTankSize() * 10))
	{
		turning = true;
	}
	else
		turning = false;


	if (turning)
	{
		glm::vec3 direction = glm::vec3(0, 0, 0) - _owner->getLocalPosition();

	}
	else
	{
		ApplyRules();

		_owner->translate(glm::vec3(0, 0, -0.10f));
	}
}

void FlockingBehaviour::ApplyRules()
{
	glm::vec3 vcentre = glm::vec3(0, 0, 0);
	glm::vec3 vavoid = glm::vec3(0, 0, 0);
	float gSpeed = 0.1f;

	glm::vec3 goal = fishtank->goalPosition;

	float dist = 0;

	int groupSize = 0;

	for (int i = 0; i < fishtank->getFishCount(); i++)
	{
		GameObject * curObject = fishtank->allFish->at(i);
		if (curObject != _owner)
		{
			dist = glm::distance(curObject->getWorldPosition(), _owner->getWorldPosition());

			if (dist <= neighbourDistance)
			{
				vcentre += curObject->getWorldPosition();
				groupSize++;

				if (dist < 4.0f)
				{
					vavoid = vavoid + (_owner->getWorldPosition() - curObject->getWorldPosition());
				}
			}
		}
	}

	if (groupSize > 0)
	{
		vcentre = (vcentre / groupSize) + (fishtank->goalPosition - _owner->getWorldPosition());
		speed = gSpeed / groupSize;

		glm::vec3 direction = (vcentre + vavoid) - _owner->getWorldPosition();

		if (direction.length() > 0)
		{
			glm::vec3 WorldPos = _owner->getWorldPosition();

			//glm::quat currentDir = glm::quat_cast(_owner->getTransform());

			//std::cout << currentDir << std::endl;

			//glm::mat4 newMat = glm::eulerAngleXYZ(direction.x, direction.y, direction.z);
			glm::mat4 newMat = glm::inverse(glm::lookAt(_owner->getLocalPosition(), _owner->getLocalPosition() + direction, glm::vec3(0, 1, 0)));
			//glm::quat newDir = glm::quat_cast(newMat);

			//std::cout << _owner->getLocalPosition() << std::endl;

			//glm::mat4 RotationMatrix = glm::mat4_cast(glm::slerp(currentDir, newDir, 0.10f));

			_owner->setTransform(newMat);
			//_owner->translate(WorldPos);
		}
	}
}
