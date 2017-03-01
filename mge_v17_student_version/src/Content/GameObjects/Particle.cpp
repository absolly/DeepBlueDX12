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
	direction = pDirection;
	_owner = pOwner;
}

void Particle::update(float pStep)
{
	if (_duration < 1)
	{
		//_owner->DeadParticles->push_back(this);
		//auto it = std::find(_owner->GetParticles().begin(), _owner->GetParticles().end(), this);
		//_owner->GetParticles().erase(it);
	}
	
	translate(direction);
	_duration--;
}

void Particle::ResetDuration(float pDuration)
{
	_duration = pDuration;
}


Particle::~Particle()
{
}