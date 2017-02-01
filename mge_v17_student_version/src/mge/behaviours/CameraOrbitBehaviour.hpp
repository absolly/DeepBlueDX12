#ifndef CAMERAORBITBEHAVIOUR_H
#define CAMERAORBITBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"

#include <glm.hpp>
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Keyboard.hpp>


class CameraOrbitBehaviour : public AbstractBehaviour
{
    public:
        CameraOrbitBehaviour(float pDistance, float pMinTiltAngle, float pMaxTiltAngle, float pRotSpeed, GameObject * pTarget);
        virtual ~CameraOrbitBehaviour();

        virtual void update(float pStep);

    private:
        GameObject * _target;   //what are we looking at?
        float _distance;
        float _minTiltAngle;
        float _maxTiltAngle;
        float _rotSpeed;

        CameraOrbitBehaviour(const CameraOrbitBehaviour&);
        CameraOrbitBehaviour& operator=(const CameraOrbitBehaviour&);
};

#endif // CAMERAORBITBEHAVIOUR_H
