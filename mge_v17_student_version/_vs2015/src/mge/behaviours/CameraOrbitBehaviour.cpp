#include "CameraOrbitBehaviour.h"
#include <iostream>
#include <glm.hpp>
#include "mge\core\GameObject.hpp"

CameraOrbitBehaviour::CameraOrbitBehaviour(sf::RenderWindow * window, GameObject * target, float distance, float minTiltAngle, float maxTiltAngle, float rotationSpeed) :
	_window(window), _target(target), _distance(distance), _minTiltAngle(minTiltAngle), _maxTiltAngle(maxTiltAngle), _rotationSpeed(rotationSpeed)
{
	_horizontalRotation = 0;
}

void CameraOrbitBehaviour::update(float pStep)
{
	/*sf::Vector2i mousePosition = sf::Mouse::getPosition(*_window);
	sf::Vector2u windowSize = _window->getSize();
	sf::Vector2f normalizedMousePosition = sf::Vector2f(mousePosition.x / (float)windowSize.x * glm::two_pi<float>(), -1 + mousePosition.y / ((float)windowSize.y/2));

	float normalizedMouseX = mousePosition.x / (float)windowSize.x;
	float mouseXBetweenZeroAnd2Pi = normalizedMouseX * glm::two_pi<float>();

	float distance = 10;
	float minTiltAngle = -5;
	float maxTiltAngle = 5;
	float rotationSpeed = 10;

	float moveSpeed = rotationSpeed*normalizedMousePosition.x;
	float yPositionBetweenMinusOneAnd1 = normalizedMousePosition.y;

	glm::vec3 cameraOffset = glm::vec3(sin(mouseXBetweenZeroAnd2Pi), 0, cos(mouseXBetweenZeroAnd2Pi));
	cameraOffset = glm::normalize(cameraOffset);
	cameraOffset = cameraOffset * distance;
	cameraOffset.y = yPositionBetweenMinusOneAnd1 * maxTiltAngle;
	glm::vec3 location = _target->getWorldPosition() + cameraOffset;

	//manually
	//the other way then you would expect, the camera is flipped over the z axis we are staring out of its a.. basically ;)
	//this is due to how the perspective matrix is set up
	glm::vec3 forward = glm::normalize(location - _target->getWorldPosition());
	glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), forward);
	glm::vec3 up = glm::cross(forward, right);

	_owner->setTransform(
		glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(location, 1))
		//glm::mat4(_owner->getParent()->getTransform()[0], _owner->getParent()->getTransform()[1], _owner->getParent()->getTransform()[2], glm::vec4(_owner->getLocalPosition(), 1) )
	);

	std::cout << "Distance: " << glm::distance(_owner->getWorldPosition(), _target->getWorldPosition()) << std::endl;*/

	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_window);
	sf::Vector2u windowSize = _window->getSize();
	//Normalize the mouse position so it's between -1 and 1
	sf::Vector2f normalizedMousePosition = sf::Vector2f(-1 + mousePosition.x / (windowSize.x / 2.0f), 
														-1 + mousePosition.y / (windowSize.y / 2.0f));


	float verticalRotation = glm::max(glm::min(normalizedMousePosition.y * 180, -_minTiltAngle), -_maxTiltAngle);
	
	_horizontalRotation += normalizedMousePosition.x *_rotationSpeed;
	
	glm::mat4 matrixMovedOnZAxis = glm::translate(glm::vec3(0, 0, _distance));
	glm::mat4 matrixMovedOnXAxis = glm::rotate(glm::radians(verticalRotation), glm::vec3(1, 0, 0));
	glm::mat4 matrixMovedOnYAxis = glm::rotate(glm::radians(_horizontalRotation), glm::vec3(0, 1, 0));
	glm::mat4 matrixFromTargetPosition = glm::translate(_target->getWorldPosition());

	_owner->setTransform(
		glm::mat4 (
			matrixFromTargetPosition * //Move relative to the target position
			matrixMovedOnYAxis * //Move horizontally
			matrixMovedOnXAxis * //Move vertically
			matrixMovedOnZAxis //Move forward
		)
	);
}
