#ifndef ROTATINGBEHAVIOUR_H
#define ROTATINGBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
/**
 * Simply rotates the object around its origin
 */
class RotatingBehaviour : public AbstractBehaviour
{
	public:
		RotatingBehaviour(glm::vec3 pRotation = glm::vec3(1,1,0));
		virtual ~RotatingBehaviour();

		virtual void update(float pStep);
		glm::vec3 rotation;
};

#endif // ROTATINGBEHAVIOUR_H
