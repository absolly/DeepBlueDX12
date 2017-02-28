#include "BoatFollowingBehaviour.hpp"

BoatFollowingBehaviour::BoatFollowingBehaviour(GameObject * pTarget, float surfaceHeight): AbstractBehaviour(), _target(pTarget), _surfaceHeight(surfaceHeight)
{
    //ctor
}

BoatFollowingBehaviour::~BoatFollowingBehaviour()
{
    //dtor
}

void BoatFollowingBehaviour::update(float pStep)
{
	glm::vec3 targetWorldPosition = _target->getWorldPosition();
	float xOffset = 5;
	float zOffset = -21;

	targetWorldPosition.x += xOffset;
	targetWorldPosition.z += zOffset;

	targetWorldPosition.y = _surfaceHeight;

    _owner->setLocalPosition(targetWorldPosition);
}

float BoatFollowingBehaviour::moveTowards(float current, float target, float speed)
{
	int direction = glm::sign(target - current);
	current += speed * direction;
	if (direction != glm::sign(target - current))
		current = target;
	return current;
}

float BoatFollowingBehaviour::smoothStep(float from, float to, float t)
{
	//t is between 0 and 1
	t = glm::clamp(t, 0.0f, 1.0f);
	float percentage = glm::sin(t*glm::pi<float>());
	return from + (to - from) * percentage;
}