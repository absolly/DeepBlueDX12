#ifndef FLOCKINGBEHAVIOUR_H
#define FLOCKINGBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include "Content/GameObjects/FishTank.hpp"
#include <random>
using namespace std;

/**
 * KeysBehaviour allows you to move an object using the keyboard in its own local space.
 * Left right turns, forward back moves.
 */
class FlockingBehaviour : public AbstractBehaviour
{
	public:
		FlockingBehaviour(FishTank * pFishtank, glm::vec3 pScale = glm::vec3(1,1,1));
		virtual ~FlockingBehaviour();
		virtual void update( float pStep );
		float speed = 0.4f;
		float rotationSpeed = 4.0f;
		glm::vec3 averageHeading;
		glm::vec3 averagePosition;
		float _speed = 1;
		float neighbourDistance = 8.0f;

    private:
		bool turning = false;
		void ApplyRules();
		void InterPolateDirection(glm::vec3 pDirection);
		void InverseDirection();
		FishTank * fishtank;
		glm::vec3 _scale;
};

#endif // KEYSBEHAVIOUR_H
