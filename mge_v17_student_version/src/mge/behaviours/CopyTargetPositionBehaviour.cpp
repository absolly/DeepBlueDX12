#include "CopyTargetPositionBehaviour.hpp"

CopyTargetPositionBehaviour::CopyTargetPositionBehaviour(GameObject * pTarget): AbstractBehaviour(), _target(pTarget)
{
    //ctor
}

CopyTargetPositionBehaviour::~CopyTargetPositionBehaviour()
{
    //dtor
}

void CopyTargetPositionBehaviour::update(float pStep)
{
    _owner->setLocalPosition(_target->getWorldPosition());
}
