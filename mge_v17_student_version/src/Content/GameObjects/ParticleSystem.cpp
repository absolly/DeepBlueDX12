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
#include "Content\GameObjects\Particle.hpp"
#include "mge/materials/TextureMaterial.hpp"


ParticleSystem::ParticleSystem(glm::vec3 pPosition, std::string pName) : GameObject(pName ,pPosition)
{	
	_startPosition = pPosition;
	_spawnRate = 1;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(float pStep)
{
	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(0, 1000);

	if(particles.size() < 2000)
	{
		for (size_t i = 0; i < _spawnRate; i++)
		{
			if (DeadParticles.size() > 0)
			{
				Particle * particle = *DeadParticles.begin();
				particle->setLocalPosition(glm::vec3(_startPosition.x, _startPosition.y, _startPosition.z));
				particle->setDuration(duration);
				particles.push_back(particle);
				DeadParticles.pop_back();
			}
			else
			{
				GameObject * particle = new Particle(glm::vec3(_startPosition.x, _startPosition.y, _startPosition.z),"particle", glm::vec3(dis(gen) / 100000.0f,0.1f, dis(gen) / 100000.0f),duration, this, _currentIndex);
				particles.push_back(particle);
				_currentIndex++;
			}
		}
	}

	//for (GameObject * partic : particles) {
	//	Particle * part = dynamic_cast<Particle*>(partic);
	//	if (!part->updateParticle(pStep))
	//	{
	//		break;
	//	}
	//}
	
	for (std::vector<GameObject *>::iterator it = particles.begin(); it != particles.end();)
	{
		Particle * object = dynamic_cast<Particle*>(*it);

		if (object->getDuration() < 0.1f)
		{
			DeadParticles.push_back(object);
			particles.erase(it);
		}
		else
		{
			object->update(pStep);
			++it;
		}


	}
}

std::vector<GameObject *> ParticleSystem::GetParticles()
{
	return particles;
}