#pragma once

#include "mge/behaviours/AbstractBehaviour.hpp"

#include <glm.hpp>
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Keyboard.hpp>


class BoatFollowingBehaviour : public AbstractBehaviour
{
    public:
		BoatFollowingBehaviour(GameObject * pTarget, float surfaceHeight);
        virtual ~BoatFollowingBehaviour();

        virtual void update(float pStep);

		float moveTowards(float current, float target, float speed);

		float smoothStep(float from, float to, float t);

    private:
        GameObject * _target;   //what are we looking at?
		float _surfaceHeight;

		BoatFollowingBehaviour(const BoatFollowingBehaviour&);
		BoatFollowingBehaviour& operator=(const BoatFollowingBehaviour&);
};