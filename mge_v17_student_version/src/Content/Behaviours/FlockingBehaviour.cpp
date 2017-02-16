#include "Content/Behaviours/FlockingBehaviour.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <random>
using namespace std;
#include "Content/GameObjects/FishTank.hpp"


FlockingBehaviour::FlockingBehaviour(FishTank * pFishtank, glm::vec3 pScale) : AbstractBehaviour()
{
	fishtank = pFishtank;
	srand(time(NULL));
	_scale = pScale;
}

FlockingBehaviour::~FlockingBehaviour()
{

}

void FlockingBehaviour::update(float pStep)
{
	std::cout << "update .." << std::endl;
	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(0, 15);

	_speed = (dis(gen) / 2.5) + 3.0f;

	if (glm::distance(_owner->getWorldPosition(), glm::vec3(0, 0, 0)) >= (fishtank->getTankSize() * 2))
	{
		turning = true;
	}
	else
	{
		turning = false;
	}


	if (turning)
	{
		InverseDirection();
	}
	else
	{
		if(dis(gen) == 1)
			ApplyRules();
	}

	_owner->translate(glm::vec3(0, 0, -_speed * 0.02f));

}

void FlockingBehaviour::ApplyRules()
{
	glm::vec3 vcentre = glm::vec3(0, 0, 0);
	glm::vec3 vavoid = glm::vec3(0, 0, 0);
	glm::vec3 ownerPosition = _owner->getWorldPosition();
	float gSpeed = 0.1f;

	glm::vec3 goal = fishtank->goalPosition;

	float dist = 0;

	int groupSize = 0;

	for (int i = 0; i < fishtank->getFishCount(); i++)
	{
		GameObject * curObject = fishtank->allFish->at(i);
		if (curObject != _owner)
		{
			dist = glm::distance(curObject->getWorldPosition(), ownerPosition);

			if (dist <= neighbourDistance)
			{
				vcentre += curObject->getWorldPosition();
				groupSize++;

				if (dist < 3.0f)
				{
					vavoid = vavoid + (ownerPosition - curObject->getWorldPosition());
				}

				if (glm::distance(fishtank->goalPosition, ownerPosition) < 8.0f)
				{
					fishtank->SetNewGoal();
				}

				std::vector<AbstractBehaviour*> behaviours = curObject->getBehaviours();
				for (AbstractBehaviour * behaviour : behaviours)
				{
					gSpeed += dynamic_cast<FlockingBehaviour*>(behaviour)->_speed;
				}
			}
		}
	}

	if (groupSize > 0)
	{
		vcentre = (vcentre / groupSize) + (fishtank->goalPosition - ownerPosition);

		//std::cout << glm::distance(ownerPosition,vcentre) << std::endl;

	/*	if (_speed > 10.0f)
			_speed = 10.0f;*/

		glm::vec3 direction = (vcentre + vavoid) - ownerPosition;

		if (direction.length() > 0)
		{
			InterPolateDirection(direction);
		}
	}
}

void FlockingBehaviour::InterPolateDirection(glm::vec3 pDirection)
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
	_owner->scale(_scale);
}

void FlockingBehaviour::InverseDirection()
{
	glm::vec3 LocalPos = _owner->getLocalPosition();

	glm::quat currentDir = glm::quat_cast(_owner->getTransform());

	glm::vec3 OriginDirection = _owner->getLocalPosition() * currentDir;

	//glm::vec3 newDirection = glm::normalize(OriginDirection) + glm::normalize((direction * 0.002f));

	//std::cout << currentDir << std::endl;

	//glm::mat4 newMat = glm::eulerAngleXYZ(direction.x, direction.y, direction.z);
	glm::mat4 newMat = glm::inverse(glm::lookAt(_owner->getLocalPosition(), fishtank->goalPosition, glm::vec3(0, 1, 0)));
	glm::quat newDir = glm::quat_cast(newMat);

	//std::cout << _owner->getLocalPosition() << std::endl;

	glm::mat4 RotationMatrix = glm::mat4_cast(glm::slerp(currentDir, newDir, 0.02f));

	_owner->setTransform(RotationMatrix);
	_owner->setLocalPosition(LocalPos);
	_owner->scale(_scale);

}
