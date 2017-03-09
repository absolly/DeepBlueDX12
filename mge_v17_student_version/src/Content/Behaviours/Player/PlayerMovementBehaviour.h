#pragma once

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "SFML\Window.hpp"
#include "mge\materials\TextureMaterial.hpp" 
#include "Content\GameObjects\ParticleSystem.hpp"

class BillBoard;
class GameObject;
class Player;
class Light;

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
	float _diverMinMoveSpeed = -12.5f;
	float _diverMinMoveSpeedBoost = -12.5f * 1.25f;
	float _diverMaxMoveSpeed = 25;
	float _diverMaxMoveSpeedBoost = 25 * 1.5f;
	float _diverAcceleration = 40;
	float _diverAccelerationBoost = 40 * 1.25f;
	float _diverDeceleration = 40;
	float _diverDecelerationBoost = 40 * 1.25f;

	float _scooterMinMoveSpeed = -25;
	float _scooterMinMoveSpeedBoost = -25 * 1.5f;
	float _scooterMaxMoveSpeed = 50;
	float _scooterMaxMoveSpeedBoost = 50 * 1.5f;
	float _scooterAcceleration = 40;
	float _scooterAccelerationBoost = 40 * 1.5f;
	float _scooterDeceleration = 40;
	float _scooterDecelerationBoost = 40 * 1.5f;

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
	float _currentMoveUpSpeed = 0;

	//[Header("Pitch rotation")]
	float _minPitchRotationSpeed = 50;
	float _maxPitchRotationSpeed = 150;
	float _currentPitch = 0;

	//[Header("Yaw rotation")]
	float _minYawRotationSpeed = 50;
	float _maxYawRotationSpeed = 150;
	float _currentYaw = 0;

	//[Header("Roll rotation")]
	float _minRollRotationSpeed = 10;
	float _maxRollRotationSpeed = 50;
	float _maxRollRotation = 10;
	float _rollRotationSpeedMultiplier = 20;
	float _currentRoll = 0;

	float _diverSensitivity = 0.05f;
	float _scooterSensitivity = 0.02f;

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
	Light* spotlight;
	bool _scooterEquiped = true;
	void unEquipScooter();
	void equipScooter();
	ParticleSystem * particleSystem;

	float moveTowards(float current, float target, float speed);
};