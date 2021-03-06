#include "BoatFollowBehaviour.h"
#include "Content\Core\Input.h"
#include "Content/TestScene.hpp"

BoatFollowBehaviour::BoatFollowBehaviour(GameObject * pTarget) : _target(pTarget)
{
	TestScene::resetEvent.bind(this, &BoatFollowBehaviour::reset);
}


BoatFollowBehaviour::~BoatFollowBehaviour()
{
}

void BoatFollowBehaviour::update(float pStep)
{
	_owner->scale(glm::vec3(10.f, 10.f, 10.f));
	glm::vec3 diff = _owner->getWorldPosition() - _target->getWorldPosition();
	diff.y = 0;
	float speed = (-pStep * (glm::length(diff) - 50)) * 0.1;
	if (glm::length(diff) > 0)
		InterPolateDirection(diff, speed);
	_owner->translate(glm::vec3(0, 0, speed));
	_owner->scale(glm::vec3(0.1f, 0.1f, 0.1f));
}



void BoatFollowBehaviour::InterPolateDirection(glm::vec3 pDirection, float pSpeed)
{
	glm::vec3 LocalPos = _owner->getLocalPosition();

	glm::quat currentDir = glm::quat_cast(_owner->getTransform());

	//glm::vec3 OriginDirection = _owner->getLocalPosition() * currentDir;

	//glm::vec3 newDirection = glm::normalize(OriginDirection) + glm::normalize((direction * 0.002f));

	//std::cout << currentDir << std::endl;

	//glm::mat4 newMat = glm::eulerAngleXYZ(direction.x, direction.y, direction.z);
	glm::mat4 newMat = glm::inverse(glm::lookAt(LocalPos + pDirection, LocalPos, glm::vec3(0, 1, 0)));
	glm::quat newDir = glm::quat_cast(newMat);

	//std::cout << _owner->getLocalPosition() << std::endl;

	glm::mat4 RotationMatrix = glm::mat4_cast(glm::slerp(currentDir, newDir, (pSpeed < -.05) ? 0.006f : 0.0001f ));

	_owner->setTransform(RotationMatrix);
	_owner->setLocalPosition(LocalPos);
	if (Input::getKeyDown(sf::Keyboard::I))
	{
		std::cout << std::endl << "Boat follow behaviour matrix after rotating: " << std::endl << _owner->getTransform() << std::endl << std::endl;
	}
}

float BoatFollowBehaviour::clip(float n, float lower, float upper)
{
	n = (n > lower) * n + !(n > lower) * lower;
	return (n < upper) * n + !(n < upper) * upper;
}

void BoatFollowBehaviour::reset()
{
	std::cout << std::endl << "Reset boat" << std::endl;

	_owner->setLocalPosition(glm::vec3(-2068, 720, 541));
}
