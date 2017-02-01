#ifndef LookAt_H
#define LookAt_H

#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * Keeps the owner looking at the given target.
 */
class LookAt : public AbstractBehaviour
{
    public:
        LookAt (GameObject * pTarget);
        LookAt (glm::vec3 pTargetPos);
        virtual ~LookAt();

        virtual void update(float pStep);

    private:
        GameObject * _target;   //what are we looking at?
        glm::vec3 _targetPos;
        LookAt(const LookAt&);
        LookAt& operator=(const LookAt&);

};

#endif // LookAt_H
