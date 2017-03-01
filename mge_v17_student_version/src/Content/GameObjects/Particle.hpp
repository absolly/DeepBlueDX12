#ifndef PARTICLE_H
#define PARTICLE_H

#include <mge/core/GameObject.hpp>
#include "Content/GameObjects/ParticleSystem.hpp"
#include <glm.hpp>
#include <cassert>
#include <iostream>

class Particle : public GameObject
{
public:
	Particle(glm::vec3 pPosition, std::string pName, glm::vec3 pDirection, float pDuration, ParticleSystem * pOwner, int pIndex);
	void ResetDuration(float pDuration);
	virtual ~Particle();
	void update(float pStep);
private:
	int _index = 0;
	ParticleSystem * _owner;
	glm::vec3 direction;
	float _duration;
};

#endif // GAMEOBJECT_H
