#pragma once

#include "AbstractBehaviour.hpp"
#include "SFML\Graphics.hpp"

class CameraOrbitBehaviour : public AbstractBehaviour
{
public:
	CameraOrbitBehaviour(sf::RenderWindow * window, GameObject * target, float distance, float minTiltAngle, float maxTiltAngle, float rotationSpeed);

	virtual void update(float pStep);
private:

	GameObject * _target;
	sf::RenderWindow * _window;
	float _horizontalRotation;
	float _distance;
	float _minTiltAngle;
	float _maxTiltAngle;
	float _rotationSpeed;
};