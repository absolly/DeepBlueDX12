#ifndef CopyTargetPositionBehaviour_H
#define CopyTargetPositionBehaviour_H

#include "mge/behaviours/AbstractBehaviour.hpp"

#include <glm.hpp>
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Keyboard.hpp>


class CopyTargetPositionBehaviour : public AbstractBehaviour
{
    public:
		CopyTargetPositionBehaviour(GameObject * pTarget, glm::bvec3 pAxis = glm::bvec3(1,1,1));
        virtual ~CopyTargetPositionBehaviour();

        virtual void update(float pStep);

    private:
        GameObject * _target;   //what are we looking at?
		glm::bvec3 _axis;
		CopyTargetPositionBehaviour(const CopyTargetPositionBehaviour&);
		CopyTargetPositionBehaviour& operator=(const CopyTargetPositionBehaviour&);
};

#endif // CopyTargetPositionBehaviour_H
