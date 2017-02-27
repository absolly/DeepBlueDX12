#include "Player.h"
#include "mge\materials\TextureMaterial.hpp"
#include "mge\core\Mesh.hpp"
#include "mge\core\Physics\CollisionBehaviour.h"
#include "mge\core\World.hpp"
#include "mge\core\Physics\PhysicsWorld.h"

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

	Mesh* cubeMeshF = Mesh::load(Config::MGE_MODEL_PATH + "cube_flat.obj");
	setMesh(cubeMeshF);
	AbstractMaterial* material = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"), 1, 10);
	setMaterial(material);
		
	_playerMovementBehaviour = new PlayerMovementBehaviour(*this);
	addBehaviour(_playerMovementBehaviour);
	//addCollider(BoxColliderArgs(btVector3(1, 1, 1)), false, true);
	//addRigidBody(1);
	//setLocalPosition(glm::vec3(10, 10, 10));
}

Player::~Player()
{
	delete &coolFloatEvent;
}
