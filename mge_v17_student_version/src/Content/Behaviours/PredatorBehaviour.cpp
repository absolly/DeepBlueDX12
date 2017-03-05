#include "PredatorBehaviour.h"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"
#include "mge/core/Physics/PhysicsWorld.h"
#include "mge\core\World.hpp" 
#include "Content/Core/Input.h"
#include "Content/Hud/Hud.hpp"
#include "Content/Behaviours/Player/PlayerFishAbilityBehaviour.h"

PredatorBehaviour::PredatorBehaviour(GameObject * pTarget, std::vector<glm::vec3> pWaypoints, World * pWorld) : _waypoints(pWaypoints), _target(pTarget), _world(pWorld)
{
	cubeMeshF = Mesh::load(Config::MGE_MODEL_PATH + "cube_flat.obj");
	colorMaterial2 = new ColorMaterial((glm::vec3(0.1, 1, 0.1)));
	for (int i = 0; i < 16; i++) {
		_crumbObjects[i] = new GameObject("", glm::vec3(0, 0, 0));
		_crumbObjects[i]->setMesh(cubeMeshF);
		_crumbObjects[i]->setMaterial(colorMaterial2);
		_crumbObjects[i]->scale(glm::vec3(0.1f));
		pWorld->add(_crumbObjects[i]);
		_crumbObjects[i]->setParent(NULL);
	}
	for each(glm::vec3 waypoint in _waypoints) {
		GameObject* go = new GameObject("", waypoint);
		go->setMesh(cubeMeshF);
		go->setMaterial(colorMaterial2);
		pWorld->add(go);
		go->setParent(NULL);
		_debugMarkers.push_back(go);
	}


}

PredatorBehaviour::~PredatorBehaviour()
{
}

void PredatorBehaviour::update(float pStep)
{
	if (_ownerMat == nullptr)
		_ownerMat = dynamic_cast<LitWaveMaterial*>(_owner->getMaterial());

	if (Input::getKeyDown(sf::Keyboard::F3)) {
		debug = !debug;
		if (debug) {
			for each(GameObject* go in _debugMarkers)
				go->setParent(_world);
			for each(GameObject* go in _crumbObjects)
				go->setParent(_world);
			for each(GameObject* go in _returnPathMarkers)
				go->setParent(_world);

		}
		else {
			for each(GameObject* go in _debugMarkers)
				go->setParent(NULL);
			for each(GameObject* go in _crumbObjects)
				go->setParent(NULL);
			for each(GameObject* go in _returnPathMarkers)
				go->setParent(NULL);
		}
	}
	_crumbCooldown--;
	if (_crumbCooldown <= 0) {
		_crumbs[crumbHead] = _target->getWorldPosition();
		_crumbObjects[crumbHead]->setLocalPosition(_target->getWorldPosition());
		crumbHead = (crumbHead + 1) % 16;
		_crumbCooldown = 20;
		//std::cout << _owner->getWorldPosition() << std::endl;
	}
	glm::vec3 closest = glm::vec3(0, 0, 0);
	glm::vec3 mypos = _owner->getWorldPosition();
	float clostestDist = -1;
	bool followingCrumbs = false;
	if (!_target->getBehaviour<PlayerFishAbilityBehaviour>()->getIsProtected())
		for (int i = 0; i < 16; i++) {

			if (glm::distance(mypos, _crumbs[(i + crumbHead) % 16]) < 10)
			{
				closest = _crumbs[(i + crumbHead) % 16];
				followingCrumbs = true;
			}
			else if (!followingCrumbs && glm::distance(mypos, _crumbs[(i + crumbHead) % 16]) < 100) {
				btVector3 Start = btVector3(mypos.x, mypos.y, mypos.z);
				btVector3 End = btVector3(_crumbs[(i + crumbHead) % 16].x, _crumbs[(i + crumbHead) % 16].y, _crumbs[(i + crumbHead) % 16].z);
				btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

				// Perform raycast
				World::physics->rayTest(Start, End, RayCallback);
#define BIT(x) (1<<(x))
				if (!RayCallback.hasHit() || RayCallback.m_collisionFilterMask == BIT(0)) {
					//std::cout << "no hit!" << std::endl;
					closest = _crumbs[(i + crumbHead) % 16];

					// Do some clever stuff here
				}

			}
		}

	if (closest != glm::vec3(0, 0, 0)) {
		_speed = 0.2;
		//std::cout << "following player" << std::endl;
		_targetPos = closest;
		if (_returnPath.size() == 0 || glm::distance(_returnPath.top(), _owner->getWorldPosition()) >= 5) {
			GameObject* go = new GameObject("", _owner->getWorldPosition());
			go->setMesh(cubeMeshF);
			go->setMaterial(colorMaterial2);
			_world->add(go);
			if (!debug)
				go->setParent(NULL);
			_returnPathMarkers.push_back(go);
			_returnPath.push(_owner->getWorldPosition());

		}
	}
	else if (_returnPath.size() > 1) {
		_speed = 0.1;
		if (glm::distance(_returnPath.top(), _owner->getWorldPosition()) < 3) {
			_returnPath.pop();
			delete _returnPathMarkers.back();
			_returnPathMarkers.pop_back();
		}
		_targetPos = _returnPath.top();
	}
	else if (glm::distance(_owner->getWorldPosition(), _waypoints[_currentWaypoint]) > 1) {
		_speed = 0.1;
		//std::cout << "navigating to waypoint " << _currentWaypoint << " current distance: " << glm::distance(_owner->getWorldPosition(), _waypoints[_currentWaypoint]) << std::endl;
		_targetPos = _waypoints[_currentWaypoint];
	}
	else {
		_speed = 0.1;
		//std::cout << "navigating to next waypoint " << _currentWaypoint << std::endl;
		_currentWaypoint++;
		_currentWaypoint = _currentWaypoint % _waypoints.size();
		_targetPos = _waypoints[_currentWaypoint];
	}
	InterPolateDirection(_owner->getWorldPosition() - _targetPos);
	_ownerMat->speed = _speed;
	_owner->translate(glm::vec3(0, 0, _speed * pStep * 100));
	if (glm::distance(mypos, _target->getWorldPosition()) < 5)
		Hud::getInstance()->isPlayerKilled = true;
}


void PredatorBehaviour::InterPolateDirection(glm::vec3 pDirection)
{
	glm::vec3 LocalPos = _owner->getLocalPosition();

	glm::quat currentDir = glm::quat_cast(_owner->getTransform());

	//glm::vec3 OriginDirection = _owner->getLocalPosition() * currentDir;

	//glm::vec3 newDirection = glm::normalize(OriginDirection) + glm::normalize((direction * 0.002f));

	//std::cout << currentDir << std::endl;

	//glm::mat4 newMat = glm::eulerAngleXYZ(direction.x, direction.y, direction.z);
	glm::mat4 newMat = glm::inverse(glm::lookAt(LocalPos, LocalPos + pDirection, glm::vec3(0, 1, 0)));
	glm::quat newDir = glm::quat_cast(newMat);

	//std::cout << _owner->getLocalPosition() << std::endl;

	glm::mat4 RotationMatrix = glm::mat4_cast(glm::slerp(currentDir, newDir, 0.02f));

	_owner->setTransform(RotationMatrix);
	_owner->setLocalPosition(LocalPos);
}