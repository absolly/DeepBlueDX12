#include "CameraOrbitBehaviour.hpp"

CameraOrbitBehaviour::CameraOrbitBehaviour(float pDistance, float pMinTiltAngle, float pMaxTiltAngle, float pRotSpeed, GameObject * pTarget): AbstractBehaviour(), _target(pTarget)
{
    //ctor
    _rotSpeed = pRotSpeed;
    _minTiltAngle = pMinTiltAngle;
    _maxTiltAngle = pMaxTiltAngle;
    _distance = pDistance;

}

CameraOrbitBehaviour::~CameraOrbitBehaviour()
{
    //dtor
}
void CameraOrbitBehaviour::update(float pStep)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
        _distance = 1 + (_distance/1.1f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
        _distance = 1 + (_distance*1.1f);
	// _owner->setParent(_target);
	//float radY = _rotSpeed * glm::clamp(((new sf::Mouse())->getPosition().y - (sf::VideoMode().height/2))/1000.f, _minTiltAngle/180.f, _maxTiltAngle/180.f);
	//float radX = _rotSpeed * ((new sf::Mouse())->getPosition().x - (sf::VideoMode().width/2))/1000.f;

	radX += _rotSpeed * pStep;
	float radY = glm::sin(radX) * _maxTiltAngle;

	//float x = glm::sin(3.14f * radY) * glm::cos(3.14f * radX);
	//float z = glm::sin(3.14f * radX) * glm::sin(3.14f * radY);
	//float y = glm::cos(3.14f * radY);

	_owner->setTransform(glm::translate(_target->getWorldPosition()));
	_owner->rotate(radX, glm::vec3(0, 1, 0));
	_owner->rotate(radY, glm::vec3(1, 0, 0));
	_owner->translate(glm::vec3(0, 0, -_distance));

	//_owner->setLocalPosition(_target->getLocalPosition() + _distance * glm::vec3(x,y,z));

	//two ways of doing the same thing:

	//Through GLM
	/*
	_owner->setTransform(
		glm::inverse (
			glm::lookAt (_owner->getWorldPosition(), _target->getWorldPosition(), glm::vec3(0,1,0))
		)
	);
	/**/

	//manually
	//the other way then you would expect, the camera is flipped over the z axis we are staring out of its a.. basically ;)
	//this is due to how the perspective matrix is set up


	glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _target->getLocalPosition());
	glm::vec3 right = glm::normalize( glm::cross (glm::vec3(0,1,0), forward));
	glm::vec3 up = glm::cross (forward, right);

	_owner->setTransform(
		glm::mat4 (glm::vec4(right,0), glm::vec4(up,0), glm::vec4(forward,0), glm::vec4(_owner->getLocalPosition(),1) )
	);
}
