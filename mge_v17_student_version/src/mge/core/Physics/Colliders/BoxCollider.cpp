#include "BoxCollider.h"

BoxCollider::BoxCollider(BoxColliderArgs& boxColliderArgs) :
	Collider(*new btBoxShape(boxColliderArgs.boxHalfExtends))
{

}
