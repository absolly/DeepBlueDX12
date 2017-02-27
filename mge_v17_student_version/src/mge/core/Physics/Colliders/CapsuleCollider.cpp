#include "CapsuleCollider.h"
#include "btBulletCollisionCommon.h"

CapsuleCollider::CapsuleCollider(CapsuleColliderArgs & capsuleColliderArgs, GameObject& owner, bool isTrigger, bool isStatic) :
	Collider(*new btCapsuleShape(capsuleColliderArgs.radius, capsuleColliderArgs.height), owner, isTrigger, isStatic)
{

}