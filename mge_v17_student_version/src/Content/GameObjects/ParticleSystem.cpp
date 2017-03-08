#include <cassert>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
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
	_spawnRate = 20;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(float pStep)
{
	random_device rd;

	_time += pStep;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(0, 1000);

		//if (_time % (1 / _spawnRate) < 0.01)
		
	if (fmod(_time , (1.0f / _spawnRate)) <= 0.05f)
	{
		if(EmitLeft > 0 || loop)
		{
			if (DeadParticles.size() > 0)
			{
				Particle * particle = DeadParticles.back();
				particle->setLocalPosition(glm::vec3(_startPosition.x, _startPosition.y, _startPosition.z));
				particle->setDuration(duration);
				particles.push_back(particle);
				DeadParticles.pop_back();
			}
			else
			{
				Particle * particle = new Particle(glm::vec3(_startPosition.x, _startPosition.y, _startPosition.z), "particle", glm::vec3(dis(gen) / 100000.0f, 0.1f, dis(gen) / 100000.0f), duration, this, _currentIndex);
				particle->SetStartEndScale(startScaleSize, endScaleSize);
				particles.push_back(particle);
				_currentIndex++;
			}
			EmitLeft--;
		}
	}
		

	//for (GameObject * partic : particles) {
	//	Particle * part = dynamic_cast<Particle*>(partic);
	//	if (!part->updateParticle(pStep))
	//	{
	//		break;
	//	}
	//}
	
	for (std::vector<Particle *>::iterator it = particles.begin(); it != particles.end();)
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

void ParticleSystem::Emit(int pAmount)
{
	EmitLeft += pAmount;
}


void ParticleSystem::setDirection(glm::vec3 pDirection)
{
	direction = pDirection;
}

void ParticleSystem::SetStartEndScale(float pStartscale, float pEndScale)
{
	startScaleSize = pStartscale;
	endScaleSize = pEndScale;
}

std::vector<Particle *> ParticleSystem::GetParticles()
{
	return particles;
}