#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <glm.hpp>

#include <mge/core/GameObject.hpp>

class Particle;

class ParticleSystem : public GameObject
{
public:
	ParticleSystem(glm::vec3 pPosition, std::string pName);
	virtual ~ParticleSystem();
	std::vector<Particle *> DeadParticles;
	std::vector<GameObject *> GetParticles();

private:
	int _spawnRate = 0;
	int _currentIndex = 0;
	std::vector<GameObject*> particles;
	glm::vec3 direction;
	glm::vec3 startScaleSize;
	glm::vec3 endScaleSize;
	glm::vec3 _startPosition;
	float duration = 2;
	void update(float pStep);

};

#endif // GAMEOBJECT_H
