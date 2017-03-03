#include "PlayerMovementBehaviour.h"
#include <iostream>
#include "mge\core\GameObject.hpp"
#include "Content\Core\Input.h"
#include "Content\GameObjects\Player.h"
#include "mge\Config.hpp"
#include "mge/core/Light.hpp"

PlayerMovementBehaviour::PlayerMovementBehaviour(Player& player)
{
	_previousMousePosition = sf::Mouse::getPosition();
	_currentPitch = 0;
	_currentYaw = 0;
	_currentRoll = 0;
	_currentMoveSpeed = 0;

	updateFromConfig();
	Config::onConfigUpdated.bind(this, &PlayerMovementBehaviour::updateFromConfig);


	Mesh* scooterMesh = Mesh::load(Config::MGE_MODEL_PATH + "dive_scooter.obj");

	_diveScooterMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "color.jpg"), 1, 1, Texture::load(Config::MGE_TEXTURE_PATH + "white.png"), Texture::load(Config::MGE_TEXTURE_PATH + "NormalNormalMap.png"));
	_diveScooterMaterial->isShadowCaster = false;

	_diveScooter = new GameObject("Dive Scooter");
	_diveScooter->setMesh(scooterMesh);
	_diveScooter->setMaterial(_diveScooterMaterial);
	_scooterOffsetMat = glm::scale(_scooterOffsetMat, glm::vec3(0.025f));

	Light* spotlight = new Light(Light::lightType::SPOT,"spotlight",glm::vec3(0), glm::vec3(0.4, 0.4, 0.7), 50, glm::vec3(0,0,1));
	_diveScooter->add(spotlight);
	spotlight->setTransform(glm::mat4(1.0f));
	spotlight->translate(glm::vec3(-30, 0, 0));
	spotlight->rotate(glm::radians(-90.f), glm::vec3(0, 1, 0));

	_scooterOffsetMat = glm::translate(_scooterOffsetMat, glm::vec3(0, -40, 30));
	_scooterOffsetMat = glm::rotate(_scooterOffsetMat, glm::radians(90.f), glm::vec3(0, 1, 0));
	player.add(_diveScooter);
	_diveScooter->setTransform(_scooterOffsetMat);
}

void PlayerMovementBehaviour::updateFromConfig()
{
	Config::updateValue("_minMoveSpeed", _minMoveSpeed);
	Config::updateValue("_maxMoveSpeed", _maxMoveSpeed);
	Config::updateValue("_moveAcceleration", _moveAcceleration);
	Config::updateValue("_moveDecceleration", _moveDecceleration);

	Config::updateValue("_minSideMoveSpeed", _minSideMoveSpeed);
	Config::updateValue("_maxSideMoveSpeed", _maxSideMoveSpeed);
	Config::updateValue("_moveSideAcceleration", _moveSideAcceleration);
	Config::updateValue("_moveSideDecceleration", _moveSideDecceleration);

	Config::updateValue("_minMoveUpSpeed", _minMoveUpSpeed);
	Config::updateValue("_maxMoveUpSpeed", _maxMoveUpSpeed);
	Config::updateValue("_moveUpAcceleration", _moveUpAcceleration);
	Config::updateValue("_moveUpDecceleration", _moveUpDecceleration);

	Config::updateValue("_minPitchRotationSpeed", _minPitchRotationSpeed);
	Config::updateValue("_maxPitchRotationSpeed", _maxPitchRotationSpeed);

	Config::updateValue("_minYawRotationSpeed", _minYawRotationSpeed);
	Config::updateValue("_maxYawRotationSpeed", _maxYawRotationSpeed);

	Config::updateValue("_minRollRotationSpeed", _minRollRotationSpeed);
	Config::updateValue("_maxRollRotationSpeed", _maxRollRotationSpeed);
	Config::updateValue("_maxRollRotation", _maxRollRotation);
	Config::updateValue("_rollRotationSpeedMultiplier", _rollRotationSpeedMultiplier);

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

	sf::Vector2i mousePosition = sf::Mouse::getPosition();
	sf::Vector2i mouseMovement = windowCenter - mousePosition;

	if (Input::mouseLocked)
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
	_currentMoveSpeed = glm::clamp(_currentMoveSpeed, _minMoveSpeed, _maxMoveSpeed*(_scooterEnquiped && Input::getKey(sf::Keyboard::LShift) ? 3 : 1));
	if (forwardInput != glm::sign(_currentMoveSpeed)) _currentMoveSpeed = moveTowards(_currentMoveSpeed, 0, _moveDecceleration * deltaTime);

	if (!_scooterEnquiped) {
		//Moving sideways
		float sidewayInput = (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1 : 0);
		_currentMoveSideSpeed += sidewayInput * _moveSideAcceleration * deltaTime;
		_currentMoveSideSpeed = glm::clamp(_currentMoveSideSpeed, _minSideMoveSpeed, _maxSideMoveSpeed*(Input::getKey(sf::Keyboard::LShift) ? 10 : 1));
		if (sidewayInput != glm::sign(_currentMoveSideSpeed)) _currentMoveSideSpeed = moveTowards(_currentMoveSideSpeed, 0, _moveSideDecceleration * deltaTime);

		_currentRoll -= sidewayInput * 5 * deltaTime;
		_currentRoll = glm::clamp(_currentRoll, -3.0f, 3.0f);

		if (sidewayInput != glm::sign(_currentRoll*-1))
		{
			_currentRoll = moveTowards(_currentRoll, 0, 5 * deltaTime);
		}
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
	
	if (_scooterEnquiped) {
		_diveScooter->setTransform(_scooterOffsetMat);//_owner->setTransform(transformedVector);
		_prevYaw = glm::clamp(glm::lerp(_prevYaw, mouseMovement.x * sensitivity * 4, deltaTime * 3), -10.0f, 10.0f);
		_diveScooter->rotate(glm::radians(_prevYaw), glm::vec3(0, 1, 0));
		_diveScooter->rotate(glm::radians(_currentPitch * 0.1f), glm::vec3(0, 0, 1));

		_diveScooter->translate(glm::vec3(-_currentMoveSpeed * 0.2f, _currentMoveUpSpeed * 0.2f, 0));
	}

	float totalMoveSpeed = glm::sqrt(glm::pow2(glm::sign(_currentMoveSpeed)) + glm::pow2(glm::sign(_currentMoveSideSpeed)));
	float multiplier = totalMoveSpeed > 0 ? (1 / totalMoveSpeed) : 0;
	btRigidBody* rigidBody = _owner->getParent()->getBehaviour<btRigidBody>();
	if (totalMoveSpeed != 0)
	{
		//_owner->translate(glm::vec3(0.0f, 0.0f, _currentMoveSpeed * multiplier * deltaTime));
		//_owner->translate(glm::vec3(_currentMoveSideSpeed * multiplier * deltaTime, 0.0f, 0.0f));
		//btRigidBody* rigidBody = _owner->getParent()->getBehaviour<btRigidBody>();
		//rigidBody->translate(btVector3(0, 0.0f, _currentMoveSpeed * multiplier * deltaTime));
		//rigidBody->translate(btVector3(_currentMoveSideSpeed * multiplier * deltaTime, 0.0f, 0.0f));
		//rigidBody->setLinearVelocity(btVector3(_currentMoveSideSpeed * multiplier, 0.0f, _currentMoveSpeed * multiplier * 1));

		//std::cout << "totalMoveSpeed: " << rigidBody->getWorldTransform().getOrigin()[0] << " active: " << rigidBody->isActive() << std::endl;
	}
	glm::quat glmQuaternion = glm::quat_cast(rotationMatrix);
	btQuaternion& quaternion = btQuaternion(glmQuaternion.x, glmQuaternion.y, glmQuaternion.z, glmQuaternion.w);
	rigidBody->getWorldTransform().setRotation(quaternion);

	btVector3& velocity = btVector3(_currentMoveSideSpeed * multiplier, _currentMoveUpSpeed, _currentMoveSpeed * multiplier * 1);
	velocity = quatRotate(quaternion, velocity);
	velocity += btVector3(0, -1, 0);
	rigidBody->setLinearVelocity(velocity);


	rigidBody->setAngularFactor(btVector3(0, 0, 0));
	rigidBody->setActivationState(ACTIVE_TAG);
	glm::vec3 ownerPosition = _owner->getWorldPosition();
	ownerPosition.y += _currentMoveUpSpeed *deltaTime;
	//ownerPosition.y = glm::clamp(ownerPosition.y, 1.0f, 30.0f);
	//_owner->setLocalPosition(ownerPosition);

	if (Input::getKeyDown(sf::Keyboard::I))
	{
		std::cout << "Player information: " << std::endl << "{" << std::endl;
		std::cout << "_minMoveUpSpeed: " << _minMoveSpeed << std::endl;
		std::cout << "_maxMoveUpSpeed: " << _maxMoveSpeed << std::endl;
		//std::cout << "_moveAcceleration: " << _moveAcceleration << std::endl;
		//std::cout << "_moveDecceleration: " << _moveDecceleration << std::endl;
		std::cout << "_minSideMoveSpeed: " << _minSideMoveSpeed << std::endl;
		std::cout << "_maxSideMoveSpeed: " << _maxSideMoveSpeed << std::endl;
		//std::cout << "_moveSideAcceleration: " << _moveSideAcceleration << std::endl;
		//std::cout << "_moveSideDecceleration: " << _moveSideDecceleration << std::endl;
		std::cout << "Position: " << ownerPosition.x << ", " << ownerPosition.y << ", " << ownerPosition.z << std::endl;
		std::cout << "}" << std::endl;
	}

	if (Input::getKeyDown(sf::Keyboard::Left))	_maxSideMoveSpeed -= 4;
	if (Input::getKeyDown(sf::Keyboard::Right))	_maxSideMoveSpeed += 4;
	if (Input::getKeyDown(sf::Keyboard::Down))	_maxMoveSpeed -= 4;
	if (Input::getKeyDown(sf::Keyboard::Up))	_maxMoveSpeed += 4;

	if (_scooterEnquiped && Input::getKeyDown(sf::Keyboard::F))
		UnenquipScooter();
	else if (!_scooterEnquiped && Input::getKeyDown(sf::Keyboard::F))
		EnquipScooter();

}

void PlayerMovementBehaviour::UnenquipScooter()
{
	_diveScooter->setLocalPosition(glm::vec3(0, -3, 2.0f));
	_diveScooter->scale(glm::vec3(3));
	glm::mat4 temp = _diveScooter->getWorldTransform();

	_diveScooter->setParent(_owner->getParent()->getParent()->getParent());
	_diveScooter->setTransform(temp);
	_scooterEnquiped = false;
}

void PlayerMovementBehaviour::EnquipScooter()
{
	glm::vec3 distance = (_owner->getWorldPosition() - _diveScooter->getWorldPosition());
	if (length(distance) < 5) {
		_diveScooter->setParent(_owner);
		_diveScooter->setTransform(_scooterOffsetMat);
		_scooterEnquiped = true;
	}
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