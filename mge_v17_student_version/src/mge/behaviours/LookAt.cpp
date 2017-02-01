#include <glm.hpp>
#include "mge/behaviours/LookAt.hpp"
#include "mge/core/GameObject.hpp"

LookAt::LookAt(GameObject * pTarget): AbstractBehaviour(), _target(pTarget) {
    std::cout << "look at GameObject" << std::endl;
}


LookAt::LookAt(glm::vec3 pTargetPos): AbstractBehaviour(), _targetPos(pTargetPos) {
    std::cout << "look at point" << std::endl;
    _target = nullptr;
}

LookAt::~LookAt() {
    //dtor
}

/**
 * Looks at the given target
 */
void LookAt::update(float pStep) {
    //two ways of doing the same thing:

    //Through GLM
    /**/
    if(_target != nullptr) {
        _owner->setTransform(
            glm::inverse (
                glm::lookAt (_owner->getWorldPosition(), _target->getWorldPosition(), glm::vec3(0,1,0))
            )
        );
    } else {
        _owner->setTransform(
            glm::inverse (
                glm::lookAt (_owner->getWorldPosition(), _targetPos, glm::vec3(0,1,0))
            )
        );
    }
    /**/

    //manually
    //the other way then you would expect, the camera is flipped over the z axis we are staring out of its a.. basically ;)
    //this is due to how the perspective matrix is set up
    /*
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    if(_target != nullptr) {
        forward = glm::normalize(_owner->getLocalPosition() - _target->getLocalPosition());
        right = glm::cross (glm::vec3(0,1,0), forward);
        up = glm::cross (forward, right);
    } else if(_targetPos != nullptr){
        forward = glm::normalize(_owner->getLocalPosition() - (*_targetPos));
        right = glm::cross (glm::vec3(0,1,0), forward);
        up = glm::cross (forward, right);
    }
    _owner->setTransform(
        glm::mat4 (glm::vec4(right,0), glm::vec4(up,0), glm::vec4(forward,0), glm::vec4(_owner->getLocalPosition(),1) )
    );
    */
}
