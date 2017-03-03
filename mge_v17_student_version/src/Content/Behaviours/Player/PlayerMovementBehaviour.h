#pragma once

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "SFML\Window.hpp"
#include "mge\materials\TextureMaterial.hpp" 
class GameObject;
class Player;

class PlayerMovementBehaviour : public AbstractBehaviour
{
public:
	PlayerMovementBehaviour(Player& player);
	void updateFromConfig();
	void onConfigUpdatedEvent(bool update);
	~PlayerMovementBehaviour();

	virtual void update(float pStep);
private:

	//[Header("Forward Movement")]
	float _minMoveSpeed = -13.0f;
	float _maxMoveSpeed = 25;
	float _moveAcceleration = 40;
	float _moveDecceleration = 40;
	float _currentMoveSpeed;
	//[Header("Sideway Movement")]
	float _minSideMoveSpeed = -10;
	float _maxSideMoveSpeed = 10;
	float _moveSideAcceleration = 40;
	float _moveSideDecceleration = 40;
	float _currentMoveSideSpeed;
	//[Header("Movement Floating Up")]
	float _minMoveUpSpeed = -4;
	float _maxMoveUpSpeed = 10;
	float _moveUpAcceleration = 40;
	float _moveUpDecceleration = 40;
	float _currentMoveUpSpeed;

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

	float _sensitivity = 0.05f;

	//[Header("Gravity")]
	float _inWaterGravity = 0.5f;
	float _outOfWaterGravity = 5;

	//[Header("Other")]
	GameObject* _aimPoint;
	sf::Vector2f _aimPointPosition;
	sf::Vector2i _previousMousePosition;
	
	glm::mat4 _scooterOffsetMat = glm::mat4(1.0f);
	float _prevYaw = 0;

	AbstractMaterial* _diveScooterMaterial;
	GameObject* _diveScooter;
	bool _scooterEquiped = true;
	void unEquipScooter();
	void equipScooter();

	float moveTowards(float current, float target, float speed);
};