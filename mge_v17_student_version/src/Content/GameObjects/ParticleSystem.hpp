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
	std::vector<Particle *> GetParticles();
	void setDirection(glm::vec3 pDirection);
	void SetStartEndScale(float pStartscale, float pEndScale);
	void setSpeedMultiplier(float pMultiplier);
	void Emit(int pAmount);

private:
	int _spawnRate = 0;
	float _speedMultiplier = 1.0;
	float _time = 0;
	int _currentIndex = 0;
	std::vector<Particle*> particles;
	glm::vec3 direction;
	float startScaleSize = 1.0f;
	float endScaleSize = 1.0f;
	glm::vec3 _startPosition;
	float duration = 5;
	void update(float pStep);
	bool loop = true;
	int EmitLeft = 0;

};

#endif // GAMEOBJECT_H
