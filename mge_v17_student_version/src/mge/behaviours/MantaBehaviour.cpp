#include "mge/behaviours/MantaBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <time.h>
#include <random>
using namespace std;

MantaBehaviour::MantaBehaviour(GameObject * pOwner) : AbstractBehaviour()
{
	_owner = pOwner;
	startPos = _owner->getLocalPosition();
	setGoal();
}

MantaBehaviour::~MantaBehaviour()
{
	//dtor
}

void MantaBehaviour::update(float pStep)
{
	if (glm::distance(goal, _owner->getLocalPosition()) < 30.0f)
	{
		setGoal();
	}

	glm::vec3 direction = goal - _owner->getLocalPosition();

	InterPolateDirection(direction);

	_owner->translate(glm::vec3(0, 0, -12  * pStep));
}

void MantaBehaviour::setGoal()
{

	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(-200, 200);

	goal = glm::vec3(dis(gen), dis(gen), dis(gen)) + startPos;
}

void MantaBehaviour::InterPolateDirection(glm::vec3 pDirection)
{
	glm::vec3 LocalPos = _owner->getLocalPosition();

	glm::quat currentDir = glm::quat_cast(_owner->getTransform());

	//glm::vec3 OriginDirection = _owner->getLocalPosition() * currentDir;

	//glm::vec3 newDirection = glm::normalize(OriginDirection) + glm::normalize((direction * 0.002f));

	//std::cout << currentDir << std::endl;

	//glm::mat4 newMat = glm::eulerAngleXYZ(direction.x, direction.y, direction.z);
	glm::mat4 newMat = glm::inverse(glm::lookAt(LocalPos, LocalPos + pDirection, glm::vec3(0, 1, 0)));
	glm::quat newDir = glm::quat_cast(newMat);

	//std::cout << _owner->getLocalPosition() << std::endl;

	glm::mat4 RotationMatrix = glm::mat4_cast(glm::slerp(currentDir, newDir, 0.01f));

	_owner->setTransform(RotationMatrix);
	_owner->setLocalPosition(LocalPos);
}
