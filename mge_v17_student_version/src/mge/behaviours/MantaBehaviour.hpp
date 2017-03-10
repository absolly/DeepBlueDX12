#ifndef MANTABEHAVIOUR_H
#define MANTABEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include <glm.hpp>

/**
 * Simply rotates the object around its origin
 */
class MantaBehaviour : public AbstractBehaviour
{
	public:
		MantaBehaviour(GameObject * pOwner);
		virtual ~MantaBehaviour();

		virtual void update(float pStep);

	private:
		void InterPolateDirection(glm::vec3 pDirection);
		glm::vec3 goal;
		void setGoal();
		glm::vec3 startPos;

};

#endif // ROTATINGBEHAVIOUR_H
