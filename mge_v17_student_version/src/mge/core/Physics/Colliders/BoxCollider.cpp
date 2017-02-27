#include "BoxCollider.h"

BoxCollider::BoxCollider(BoxColliderArgs& boxColliderArgs, GameObject& owner, bool isTrigger, bool isStatic) :
	Collider(*new btBoxShape(boxColliderArgs.boxHalfExtends), owner, isTrigger, isStatic)
{

}
