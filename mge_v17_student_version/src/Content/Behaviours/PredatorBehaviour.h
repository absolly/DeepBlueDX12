#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "glm.hpp"
#include <vector>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <stack>
#include "mge/materials/LitWaveMaterial.hpp"
#include "Content\GameObjects\Player.h"


class Mesh;
class AbstractMaterial;
class World;
class PredatorBehaviour : public AbstractBehaviour {
public:
	PredatorBehaviour(Player* pTarget, std::vector<glm::vec3> pWaypoints, World* pWorld);
	~PredatorBehaviour();
	virtual void update(float pStep);

private:
	float _speed = 0.1;
	Player* _player;
	GameObject* _target;
	glm::vec3 _targetPos;
	std::vector<glm::vec3> _waypoints;
	int _currentWaypoint = 0;
	glm::vec3 _crumbs[16];
	GameObject* _crumbObjects[16];
	std::vector<GameObject*> _debugMarkers;
	std::vector<GameObject*> _returnPathMarkers;
	std::stack<glm::vec3> _returnPath;
	int crumbHead = 0;
	int _crumbCooldown = 0;
	void InterPolateDirection(glm::vec3 pDirection);
	bool debug = false;
	World* _world;
	Mesh* cubeMeshF;
	AbstractMaterial* colorMaterial2;
	LitWaveMaterial* _ownerMat = nullptr;
	float _soundAttackDelayTimer = 0;
	float _soundNearbyDelayTimer = 0;
};

