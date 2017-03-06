#include <cassert>
#include <iostream>
#include <string>
#include <time.h>
#include <random>
using namespace std;

#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"
#include "Content/GameObjects/ParticleSystem.hpp"
#include "Content/GameObjects/Particle.hpp"
#include "mge/core/GameObject.hpp"

Particle::Particle(glm::vec3 pPosition, std::string pName, glm::vec3 pDirection, float pDuration, ParticleSystem * pOwner, int pIndex) : GameObject(pName, pPosition)
{
	_index = pIndex;
	_duration = pDuration;
	_OriginalDuration = pDuration;
	originalDirection = pDirection;
	direction = pDirection;
	_owner = pOwner;
}

void Particle::update(float pStep)
{
	//direction = originalDirection * (_duration / originalDirection);

	translate(direction);
	_duration -= pStep;
}

float Particle::getDuration()
{
	return _duration;
}

void Particle::setDuration(float pDuration)
{
	_duration = pDuration;
}


void Particle::ResetDuration(float pDuration)
{
	_duration = pDuration;
}


Particle::~Particle()
{
}