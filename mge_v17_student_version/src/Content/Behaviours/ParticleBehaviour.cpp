#include "Content/Behaviours/ParticleBehaviour.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <random>
using namespace std;


ParticleBehaviour::ParticleBehaviour(ParticleSystem * pParticleSystem) : AbstractBehaviour()
{
	
}

ParticleBehaviour::~ParticleBehaviour()
{

}

void ParticleBehaviour::update(float pStep)
{
	if (_duration < 0)
	{
		//add it to the list of dead particles
	}
	else
	{
		_owner->translate(_direction);
	}


	_duration -= 1;
}

void ParticleBehaviour::SetParticleAlive()
{
	_particleDied = true;
}