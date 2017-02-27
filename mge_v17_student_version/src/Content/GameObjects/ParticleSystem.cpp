#include <cassert>
#include <iostream>
#include <string>
#include <time.h>
#include <random>
using namespace std;

#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"
#include "Content/GameObjects/ParticleSystem.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/materials/TextureMaterial.hpp"

ParticleSystem::ParticleSystem(glm::vec3 pPosition, std::string pName) : GameObject(pName ,pPosition)
{	
	_startPosition = pPosition;
	_spawnRate = 1;

	DeadParticles = new std::vector<GameObject*>();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(float pStep)
{
	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(1, 10);

	if(particles.size() < 2000)
	{
		for (size_t i = 0; i < _spawnRate; i++)
		{
			if (DeadParticles->size() > 0)
			{
				//Use this one instead of a new particle damnit! RECYCLE IT COEN
			}
			else
			{
				GameObject * particle = new GameObject("particle", glm::vec3(_startPosition.x + (dis(gen) / 5), _startPosition.y + (dis(gen) / 5), _startPosition.z + (dis(gen) / 5)));
				particles.push_back(particle);
			}
		}
	}

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->translate(glm::vec3(0, 0.1f, 0));
	}
}

std::vector<GameObject *> ParticleSystem::GetParticles()
{
	return particles;
}