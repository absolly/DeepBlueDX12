#pragma once

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "SFML\Window.hpp"
class GameObject;
class Player;

class PlayerMovementBehaviour : public AbstractBehaviour
{
public:
	PlayerMovementBehaviour(Player& player);
	~PlayerMovementBehaviour();

	void coolEventToExecuteWhenPlayerDoesSomething(float deltaTime);

	virtual void update(float pStep);
private:

	//[Header("Movement")]
	float _minMoveSpeed = -5;
	float _maxMoveSpeed = 5;
	float _moveAcceleration = 3;
	float _moveDecceleration = 3;
	float _currentMoveSpeed;

	//[Header("Pitch rotation")]
	float _minPitchRotationSpeed = 50;
	float _maxPitchRotationSpeed = 150;
	float _currentPitch;

	//[Header("Yaw rotation")]
	float _minYawRotationSpeed = 50;
	float _maxYawRotationSpeed = 150;
	float _currentYaw;

	//[Header("Roll rotation")]
	float _minRollRotationSpeed = 10;
	float _maxRollRotationSpeed = 50;
	float _maxRollRotation = 10;
	float _rollRotationSpeedMultiplier = 20;
	float _currentRoll;

	//[Header("Gravity")]
	float _inWaterGravity = 0.5f;
	float _outOfWaterGravity = 5;

	//[Header("Other")]
	GameObject* _aimPoint;
	sf::Vector2f _aimPointPosition;
	sf::Vector2i _previousMousePosition;

	float moveTowards(float current, float target, float speed);
};