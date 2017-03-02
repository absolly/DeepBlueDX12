#include "CopyTargetPositionBehaviour.hpp"

CopyTargetPositionBehaviour::CopyTargetPositionBehaviour(GameObject * pTarget, glm::bvec3 pAxis) : AbstractBehaviour(), _target(pTarget), _axis(pAxis)
{
	//ctor
}

CopyTargetPositionBehaviour::~CopyTargetPositionBehaviour()
{
	//dtor
}

void CopyTargetPositionBehaviour::update(float pStep)
{
	glm::vec3 targetPos = _target->getWorldPosition();
	glm::vec3 pos = _owner->getWorldPosition();
	_owner->setLocalPosition(glm::vec3((_axis.x) ? targetPos.x : pos.x, (_axis.y) ? targetPos.y : pos.y, (_axis.z) ? targetPos.z : pos.z));
}
