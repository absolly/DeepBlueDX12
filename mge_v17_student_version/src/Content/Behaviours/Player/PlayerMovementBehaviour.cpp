#include "PlayerMovementBehaviour.h"
#include <iostream>
#include "mge\core\GameObject.hpp"
#include "Content\Core\Input.h"
#include "Content\GameObjects\Player.h"

PlayerMovementBehaviour::PlayerMovementBehaviour(Player& player)
{
	_previousMousePosition = sf::Mouse::getPosition();
	_currentPitch = 0;
	_currentYaw = 0;
	_currentRoll = 0;
	_currentMoveSpeed = 0;
}

PlayerMovementBehaviour::~PlayerMovementBehaviour()
{
}

void PlayerMovementBehaviour::update(float deltaTime)
{
	float sensitivity = 0.15f;
	float invertYAxis = false;

	//TODO Get the screen position and screensize from the actual window.
	sf::Vector2i screenResolution = sf::Vector2i(1920, 1080);
	sf::Vector2i windowSize = sf::Vector2i(1440, 810);
	sf::Vector2i windowPosition = screenResolution / 2 - windowSize / 2;
	sf::Vector2i windowCenter = windowPosition + windowSize / 2;

	if (Input::getKeyDown(sf::Keyboard::F1))
	{
		_lockMouse = !_lockMouse;
		if (_lockMouse)
		{
			sf::Mouse::setPosition(windowCenter);
		}
	}

	sf::Vector2i mousePosition = sf::Mouse::getPosition();
	sf::Vector2i mouseMovement = windowCenter - mousePosition;

	if (_lockMouse)
	{
		sf::Mouse::setPosition(windowCenter);
		_currentYaw += mouseMovement.x * sensitivity;
		_currentPitch += (invertYAxis ? 1 : -1) * mouseMovement.y * sensitivity;
		_currentPitch = glm::clamp(_currentPitch, -90.0f, 90.0f);
	}

	//YourRotationFunctionThatTakesDelta(deltaX, deltaY);



	//Moving forward
	float forwardInput = (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1 : 0);
	_currentMoveSpeed += forwardInput * _moveAcceleration * deltaTime;
	_currentMoveSpeed = glm::clamp(_currentMoveSpeed, _minMoveSpeed*(Input::getKey(sf::Keyboard::LShift) ? 10 : 1), _maxMoveSpeed*(Input::getKey(sf::Keyboard::LShift) ? 10 : 1));
	if (forwardInput != glm::sign(_currentMoveSpeed)) _currentMoveSpeed = moveTowards(_currentMoveSpeed, 0, _moveDecceleration * deltaTime);

	//Moving sideways
	float sidewayInput = (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1 : 0);
	_currentMoveSideSpeed += sidewayInput * _moveSideAcceleration * deltaTime;
	_currentMoveSideSpeed = glm::clamp(_currentMoveSideSpeed, _minSideMoveSpeed*(Input::getKey(sf::Keyboard::LShift) ? 10 : 1), _maxSideMoveSpeed*(Input::getKey(sf::Keyboard::LShift) ? 10 : 1));
	if (sidewayInput != glm::sign(_currentMoveSideSpeed)) _currentMoveSideSpeed = moveTowards(_currentMoveSideSpeed, 0, _moveSideDecceleration * deltaTime);


	_currentRoll -= sidewayInput * 5 * deltaTime;
	_currentRoll = glm::clamp(_currentRoll, -3.0f, 3.0f);

	if (sidewayInput != glm::sign(_currentRoll*-1))
	{
		_currentRoll = moveTowards(_currentRoll, 0, 5 *deltaTime);
	}

	//Moving up and down
	float upwardInput = (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 1 : 0);
	_currentMoveUpSpeed += upwardInput * _moveUpAcceleration * deltaTime;
	_currentMoveUpSpeed = glm::clamp(_currentMoveUpSpeed, _minMoveUpSpeed, _maxMoveUpSpeed);
	if (upwardInput != glm::sign(_currentMoveUpSpeed)) _currentMoveUpSpeed = moveTowards(_currentMoveUpSpeed, 0, _moveUpDecceleration * deltaTime);

	//Applying the rotation
	glm::mat4x4 translationMatrix = glm::translate(_owner->getLocalPosition());
	glm::mat4x4 rotationMatrix;// = glm::eulerAngleXYZ(glm::radians(_currentPitch), glm::radians(_currentYaw), 0.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_currentYaw), glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_currentRoll), glm::vec3(0, 0, 1));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_currentPitch), glm::vec3(1, 0, 0));

	glm::mat4x4 scaleMatrix = glm::mat4x4(1);
	glm::mat4x4 transformedVector = translationMatrix * rotationMatrix * scaleMatrix;;// *originalVector;
	_owner->setTransform(transformedVector);

	float totalMoveSpeed = glm::sqrt(glm::pow2(glm::sign(_currentMoveSpeed)) + glm::pow2(glm::sign(_currentMoveSideSpeed)));
	float multiplier = totalMoveSpeed > 0 ? (1 / totalMoveSpeed) : 0;
	if (totalMoveSpeed != 0)
	{
		_owner->translate(glm::vec3(0.0f, 0.0f, _currentMoveSpeed * multiplier * deltaTime));
		_owner->translate(glm::vec3(_currentMoveSideSpeed * multiplier * deltaTime, 0.0f, 0.0f));
	}
	
	glm::vec3 ownerPosition = _owner->getLocalPosition();
	ownerPosition.y += _currentMoveUpSpeed *deltaTime;
	//ownerPosition.y = glm::clamp(ownerPosition.y, 1.0f, 30.0f);
	_owner->setLocalPosition(ownerPosition);
}

float PlayerMovementBehaviour::moveTowards(float current, float target, float speed)
{
	int direction = glm::sign(target - current);// current < target ? 1 : current > target ? -1 : 0;
	current += speed * direction;
	if (direction != glm::sign(target - current))
	{
		current = target;
	}
	return current;
}
