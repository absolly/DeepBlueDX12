#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/core/GameObject.hpp"

RotatingBehaviour::RotatingBehaviour(glm::vec3 pRotation) : AbstractBehaviour()
{
	rotation = pRotation;
	//ctor
}

RotatingBehaviour::~RotatingBehaviour()
{
	//dtor
}

void RotatingBehaviour::update(float pStep)
{
	_owner->rotate(pStep * glm::radians(45.0f), rotation ); // rotates 45° per second
}
