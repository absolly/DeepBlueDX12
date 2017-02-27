#ifndef PARTICLEBEHAVIOUR_H
#define PARTICLEBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "Content\GameObjects\ParticleSystem.hpp"
#include "glm.hpp"
#include <random>
using namespace std;

/**
 * KeysBehaviour allows you to move an object using the keyboard in its own local space.
 * Left right turns, forward back moves.
 */
class ParticleBehaviour : public AbstractBehaviour
{
	public:
		ParticleBehaviour(ParticleSystem * pParticleSystem);
		virtual ~ParticleBehaviour();
		virtual void update( float pStep );

    private:
		float _duration;
		glm::vec3 _direction;
		bool _particleDied = false;
		void SetParticleAlive();
};

#endif // KEYSBEHAVIOUR_H
