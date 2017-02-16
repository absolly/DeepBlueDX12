#include "BoxCollider.h"

BoxCollider::BoxCollider(BoxColliderArgs& boxColliderArgs, GameObject& owner, bool isTrigger) :
	Collider(*new btBoxShape(boxColliderArgs.boxHalfExtends), owner, isTrigger)
{

}
