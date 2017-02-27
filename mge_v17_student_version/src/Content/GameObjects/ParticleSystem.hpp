#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>

#include <mge/core/GameObject.hpp>

class ParticleSystem : public GameObject
{
public:
	ParticleSystem(glm::vec3 pPosition, std::string pName);
	virtual ~ParticleSystem();
	std::vector<GameObject*> * DeadParticles;
	std::vector<GameObject *> GetParticles();

private:
	int _spawnRate = 0;
	std::vector<GameObject*> particles;
	glm::vec3 direction;
	glm::vec3 startScaleSize;
	glm::vec3 endScaleSize;
	glm::vec3 _startPosition;
	float duration;
	void update(float pStep);

};

#endif // GAMEOBJECT_H
