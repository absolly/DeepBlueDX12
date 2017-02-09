#include "PlayerMovementBehaviour.h"
#include <iostream>
#include "mge\core\GameObject.hpp"
#include "Content\Core\Input.h"
#include "Content\GameObjects\Player.h"

PlayerMovementBehaviour::PlayerMovementBehaviour(Player& player)
{
	_previousMousePosition = Input::mouseMotion; //sf::Mouse::getPosition();
	_currentPitch = 0;
	_currentYaw = 0;
	_currentRoll = 0;
	_currentMoveSpeed = 0;
	player.coolFloatEvent.bind(this, &PlayerMovementBehaviour::coolEventToExecuteWhenPlayerDoesSomething);
}

PlayerMovementBehaviour::~PlayerMovementBehaviour()
{
}

void PlayerMovementBehaviour::coolEventToExecuteWhenPlayerDoesSomething(float deltaTime)
{
	std::cout << "coolEventToExecuteWhenPlayerDoesSomething: " << deltaTime << std::endl;
}
void PlayerMovementBehaviour::update(float deltaTime)
{
	/*
	float moveSpeed = 0.0f; //default if no keys
	float turnSpeed = 0.0f;
	float flySpeed = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		moveSpeed = _maxMoveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		moveSpeed = -_maxMoveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		turnSpeed = -_maxPitchRotationSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		turnSpeed = +_maxPitchRotationSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		flySpeed = -_maxMoveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		flySpeed = +_maxMoveSpeed;
	}
	//translate the object in its own local space
	_owner->translate(glm::vec3(0.0f, flySpeed*pStep, moveSpeed*pStep));
	_owner->rotate(glm::radians(turnSpeed*pStep), glm::vec3(0.0f, 1.0f, 0.0f));*/


	//_aimPointPosition = _aimPoint.transform.localPosition;

	//should get relative mouse position to world
	sf::Vector2i mousePosition = sf::Mouse::getPosition();
	int width = 1920;
	int height = 1080;
	int deltaX = mousePosition.x - width / 2;
	int deltaY = mousePosition.y - height / 2;

	sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2)); /* could also use SetCursorPos() if you're only going to use Windows */

	//YourRotationFunctionThatTakesDelta(deltaX, deltaY);








	sf::Vector2i mouseInputRaw = sf::Vector2i(glm::sign(deltaX),glm::sign(deltaY));
	_previousMousePosition = mousePosition;
	_aimPointPosition.x = glm::clamp(_aimPointPosition.x - mouseInputRaw.x * 1	 * deltaTime, -1.0f, 1.0f);
	_aimPointPosition.y = glm::clamp(_aimPointPosition.y - mouseInputRaw.y * 1 * deltaTime, -1.0f, 1.0f);
	//std::cout << "_aimPointPosition: " << _aimPointPosition.x << ", " << _aimPointPosition.y << std::endl;
	//_aimPoint.transform.localPosition = new Vector3(aimPointPosition.x, aimPointPosition.y, _aimPoint.transform.localPosition.z);

	//Rotations
	float normalizedSpeed = 1 - _currentMoveSpeed / _maxMoveSpeed;

	//Pitch rotation
	float minMaxPitchRotationDiff = _maxPitchRotationSpeed - _minPitchRotationSpeed;
	_currentPitch += -_aimPointPosition.y * (_minPitchRotationSpeed + minMaxPitchRotationDiff * normalizedSpeed) * deltaTime;

	//Yaw rotation
	float minMaxYawRotationDiff = _maxYawRotationSpeed - _minYawRotationSpeed;
	_currentYaw += _aimPointPosition.x * (_minYawRotationSpeed + minMaxYawRotationDiff * normalizedSpeed) * deltaTime;


	//Roll rotation
	float rollDirection = (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? 1 : 0);
	//std::cout << "rollDirection: " << rollDirection << std::endl;
	float minMaxRollRotationDiff = _maxRollRotationSpeed - _minRollRotationSpeed;
	//* (_minRollRotationSpeed + minMaxRollRotationDiff * normalizedSpeed)
	_currentRoll += rollDirection * _maxRollRotationSpeed * deltaTime;

	if (_currentRoll < -_maxRollRotation - normalizedSpeed * _rollRotationSpeedMultiplier) _currentRoll = -_maxRollRotation - normalizedSpeed * _rollRotationSpeedMultiplier;
	if (_currentRoll > _maxRollRotation + normalizedSpeed * _rollRotationSpeedMultiplier) _currentRoll = _maxRollRotation + normalizedSpeed * _rollRotationSpeedMultiplier;

	float rollBackForce = 10.0f;
	if (rollDirection == 0)
	{
		_currentRoll = moveTowards(_currentRoll, 0, rollBackForce * deltaTime);
	}


	//transform.rotation = Quaternion.Euler(_currentPitch, _currentYaw, _currentRoll);
	//glm::mat4x4 rotationMatrix;

	//Moving forward
	float forwardInput = (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1 : 0);

	_currentMoveSpeed += forwardInput * _moveAcceleration * deltaTime;
	_currentMoveSpeed = glm::clamp(_currentMoveSpeed, _minMoveSpeed, _maxMoveSpeed);

	if (forwardInput == 0) _currentMoveSpeed = moveTowards(_currentMoveSpeed, 0, _moveDecceleration * deltaTime);

	//transform.position = transform.position + transform.forward * _currentMoveSpeed * Time.deltaTime;

	//glm::mat4x4 transform = glm::eulerAngleXYZ(0.0f, glm::radians( _currentYaw), 0.0f);
	//transform = glm::translate(transform, _owner->getWorldPosition());
	//transform = glm::translate(transform, _owner->getLocalPosition());
	//Apply rotation
	//std::cout << "_currentPitch: " << _currentPitch << std::endl;
	//std::cout << "_currentRoll: " << _currentRoll << std::endl;
	//std::cout << "_currentRoll: " << _currentRoll << std::endl;
	//_owner->rotate(_currentPitch * deltaTime, glm::vec3(1, 0, 0));
	//_owner->setTransform(transform);

	//glm::mat4x4 originalVector;
	glm::mat4x4 translationMatrix = glm::translate(_owner->getWorldPosition());
	glm::mat4x4 rotationMatrix;// = glm::eulerAngleXYZ(glm::radians(_currentPitch), glm::radians(_currentYaw), 0.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_currentYaw), glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_currentRoll), glm::vec3(0, 0, 1));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_currentPitch), glm::vec3(1, 0, 0));

	glm::mat4x4 scaleMatrix = glm::mat4x4(1);
	glm::mat4x4 transformedVector = translationMatrix * rotationMatrix * scaleMatrix;;// *originalVector;

	_owner->setTransform(transformedVector);
	_owner->translate(glm::vec3(0.0f, 0.0f, _currentMoveSpeed*deltaTime));
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
