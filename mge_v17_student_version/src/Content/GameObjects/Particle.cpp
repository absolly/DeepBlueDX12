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
	direction = originalDirection;
	//direction = originalDirection * (_duration / originalDirection);

	float progressPercentage = _duration / _OriginalDuration;
	_scale = startScaleSize + (endScaleSize - (progressPercentage * endScaleSize));
	
	direction -= direction * progressPercentage;

	translate(direction);
	_duration -= pStep;
}

float Particle::getDuration()
{
	return _duration;
}

float Particle::getScale()
{
	return _scale;
}

void Particle::setDuration(float pDuration)
{
	_duration = pDuration;
}


void Particle::SetStartEndScale(float pStartscale, float pEndScale)
{
	startScaleSize = pStartscale;
	endScaleSize = pEndScale;
}

void Particle::ResetDuration(float pDuration)
{
	_duration = pDuration;
}


Particle::~Particle()
{
}