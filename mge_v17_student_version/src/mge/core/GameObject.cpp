#include <cassert>
#include <iostream>
using namespace std;

#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "Content\Core\EventHandler.h"
#include "Bullet3Common\b3Vector3.h"
#include "mge\core\Physics\CollisionBehaviour.h"
#include "mge\core\Physics\RigidBody.hpp"
#include "mge\core\Physics\PhysicsWorld.h"
int GameObject::_gameobjects = 0;

GameObject::GameObject(std::string pName, glm::vec3 pPosition)
	: _name(pName), _transform(glm::translate(pPosition)),
	_parent(NULL), _children(), _mesh(NULL), _behaviours(vector<AbstractBehaviour*>()), _material(NULL), colliders(std::vector<Collider*>()), constantBufferID(_gameobjects)
{
	_gameobjects++;
}

GameObject::~GameObject()
{
	//detach all children
	//cout << "GC running on:" << _name << endl;
	if (_parent != NULL)
		_parent->remove(this);
	while (_children.size() > 0) {
		GameObject* child = _children[0];
		remove(child);
		delete child;
	}

	Collider* collider = getBehaviour<Collider>();
	if (collider != nullptr)
	{
		World::physics->removeCollisionObject(collider);
		delete collider;
	}

	RigidBody* rigidbody = getBehaviour<RigidBody>();
	if (rigidbody != nullptr)
	{
		World::physics->removeRigidBody(rigidbody);
		delete rigidbody;
	}

	colliders.clear();
	EventHandler::unbindEvents(this);
	//do not forget to delete behaviour, material, mesh, collider manually if required!
}

void GameObject::setName(std::string pName)
{
	_name = pName;
}

std::string GameObject::getName() const
{
	return _name;
}

void GameObject::setTransform(const glm::mat4& pTransform)
{
	_transform = pTransform;
}

void GameObject::setWorldTransform_TEST(const glm::mat4& pTransform)
{
	_transform = glm::inverse(getParent()->getWorldTransform()) *pTransform;
}

const glm::mat4& GameObject::getTransform() const
{
	return _transform;
}

btTransform GameObject::getBulletPhysicsTransform() const
{
	btTransform btTransform;
	glm::mat4 transform = getWorldTransform();
	glm::vec3 scale = glm::vec3(glm::length(transform[0]), glm::length(transform[1]), glm::length(transform[2]));
	//std::cout << "[Gameobject->getBulletPhysicsTransform()->transform] " << transform << std::endl;
	transform = glm::scale(transform, glm::vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
	glm::quat rotation = glm::quat_cast(transform);
	glm::vec3 position = transform[3];
	btTransform.setIdentity();
	btTransform.setOrigin(btVector3(position.x, position.y, position.z));
	btTransform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
	return btTransform;
}

glm::vec3 GameObject::getLocalScale()
{
	return glm::vec3(glm::length(_transform[0]), glm::length(_transform[1]), glm::length(_transform[2]));
}
btVector3 GameObject::getBulletPhysicsLocalScale()
{
	return btVector3(glm::length(_transform[0]), glm::length(_transform[1]), glm::length(_transform[2]));
}

void GameObject::setLocalPosition(glm::vec3 pPosition)
{
	_transform[3] = glm::vec4(pPosition, 1);
}

glm::vec3 GameObject::getLocalPosition() const
{
	return glm::vec3(_transform[3]);
}

void GameObject::setMaterial(AbstractMaterial* pMaterial)
{
	_material = pMaterial;
}

AbstractMaterial * GameObject::getMaterial() const
{
	return _material;
}

void GameObject::setMesh(Mesh* pMesh)
{
	_mesh = pMesh;
}

Mesh * GameObject::getMesh() const
{
	return _mesh;
}

void GameObject::addBehaviour(AbstractBehaviour* pBehaviour)
{
	_behaviours.push_back(pBehaviour);
	pBehaviour->setOwner(this);
}

std::vector<AbstractBehaviour*> GameObject::getBehaviours()
{
	return _behaviours;
}

Collider& GameObject::addCollider(BoxColliderArgs& colliderArgs, bool isTrigger, bool isStatic)
{
	Collider& collider = *new BoxCollider(colliderArgs, *this, isTrigger, isStatic);
	//colliders.push_back(&collider);
	addBehaviour(&collider);
	return collider;
}

Collider& GameObject::addCollider(MeshColliderArgs& colliderArgs, bool isTrigger, bool isStatic)
{
	Collider& collider = *new MeshCollider(colliderArgs, *this, isTrigger, isStatic);
	//colliders.push_back(&collider);
	addBehaviour(&collider);
	return collider;
}

Collider& GameObject::addCollider(SphereColliderArgs& colliderArgs, bool isTrigger, bool isStatic)
{
	Collider& collider = *new SphereCollider(colliderArgs, *this, isTrigger, isStatic);
	//colliders.push_back(&collider);
	addBehaviour(&collider);
	return collider;
}

Collider& GameObject::addCollider(CapsuleColliderArgs& colliderArgs, bool isTrigger, bool isStatic)
{
	Collider& collider = *new CapsuleCollider(colliderArgs, *this, isTrigger, isStatic);
	//colliders.push_back(&collider);
	addBehaviour(&collider);
	return collider;
}

/*
RigidBody& GameObject::addRigidBody(float mass, btVector3& inertia, btDefaultMotionState& defaultMotionState)
{
	if (colliders.size() == 0)
	{
		throw exception("Attempting to add a rigidbody to a GameObect that doesn't have a collider yet!");
	}
	colliders[0]->getColliderShape().calculateLocalInertia(mass, inertia);
	RigidBody& rigidBody = *new RigidBody(mass, &defaultMotionState, &colliders[0]->getColliderShape(), inertia);
	addBehaviour(&rigidBody);
	return rigidBody;
}*/

void GameObject::setParent(GameObject* pParent) {
	//remove from previous parent
	if (_parent != NULL) {
		_parent->_innerRemove(this);
		_parent = NULL;
	}

	//set new parent
	if (pParent != NULL) {
		_parent = pParent;
		_parent->_innerAdd(this);
	}
}

GameObject* GameObject::getParent() {
	return _parent;
}

void GameObject::_innerAdd(GameObject* pChild)
{
	//set new parent
	pChild->_parent = this;
	_children.push_back(pChild);
}

void GameObject::_innerRemove(GameObject* pChild) {
	for (auto i = _children.begin(); i != _children.end(); ++i) {
		if (*i == pChild) {
			(*i)->_parent = NULL;
			_children.erase(i);
			return;
		}
	}
}

void GameObject::add(GameObject* pChild) {
	pChild->setParent(this);
}

void GameObject::remove(GameObject* pChild) {
	pChild->setParent(NULL);
}

////////////

//costly operation, use with care
glm::vec3 GameObject::getWorldPosition() const
{
	return glm::vec3(getWorldTransform()[3]);
}

//costly operation, use with care
glm::mat4 GameObject::getWorldTransform() const
{
	if (_parent == NULL) return _transform;
	else return _parent->getWorldTransform() * _transform;
}

////////////

void GameObject::translate(glm::vec3 pTranslation)
{
	setTransform(glm::translate(_transform, pTranslation));
}

void GameObject::scale(glm::vec3 pScale)
{
	setTransform(glm::scale(_transform, pScale));
	for each (Collider* collider in colliders)
	{
		collider->getColliderShape().setLocalScaling(getBulletPhysicsLocalScale());
	}
}

void GameObject::rotate(float pAngle, glm::vec3 pAxis)
{
	setTransform(glm::rotate(_transform, pAngle, pAxis));
}

//all game objects are updated in a backward loop, first the behaviour is updated, then all children are updated
void GameObject::update(float pStep)
{
	for each (AbstractBehaviour* behaviour in _behaviours)
	{
		behaviour->update(pStep);
	}
	for (int i = _children.size() - 1; i >= 0; --i) {
		_children[i]->update(pStep);
	}
}

int GameObject::getChildCount() {
	return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) {
	return _children[pIndex];
}

