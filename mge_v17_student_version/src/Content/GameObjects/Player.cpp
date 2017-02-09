#include "Player.h"
#include "mge\materials\TextureMaterial.hpp"
#include "mge\core\Mesh.hpp"

Player::Player() : GameObject("Player"),
coolFloatEvent(*new Event<float>())
{
	#pragma region FutureImplementation
	/*
	float mass = 10;
	btScalar radius = 5;
	btScalar height = 10;
	btCapsuleShape* shape = new btCapsuleShape(radius, height);
	bool canMove = false;
	PhysicsBehaviour& physicsBehaviour = *new PhysicsBehaviour(shape, canMove);
	addBehaviour(physicsBehaviour);
	*/
	#pragma endregion

	Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	setMesh(cubeMeshF);
	AbstractMaterial* material = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), 1, 10);
	setMaterial(material);

	_playerMovementBehaviour = new PlayerMovementBehaviour(*this);
	addBehaviour(_playerMovementBehaviour);

	EventHandler::bindKeyDownEvent(sf::Keyboard::Space, this, &Player::onSpacePressedEvent);
	coolFloatEvent(10);
	coolFloatEvent(5);
	coolFloatEvent(3);
	//setLocalPosition(glm::vec3(10, 10, 10));
}

Player::~Player()
{

}

void Player::onSpacePressedEvent(sf::Event::KeyEvent& keyEvent)
{
	std::cout << "Someone pressed space! Player showing a message" << std::endl;
}
