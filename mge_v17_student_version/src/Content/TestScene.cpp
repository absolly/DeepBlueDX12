#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/core/PhysicsObject.hpp"
#include "mge/core/Light.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/WobbleMaterial.hpp"
#include "mge/materials/LitWaveMaterial.hpp"
#include "mge/materials/GPUinstancingMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/RigidBody.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/CameraOrbitBehaviour.hpp"
#include "Content\Behaviours\Player\DivingAnimationBehaviour.h"
#include "mge/util/DebugHud.hpp"

#include "Content/GameObjects/FishTank.hpp"

#include "mge/config.hpp"
#include "Content/TestScene.hpp"
#include <time.h>       /* time */
#include "Content\GameObjects\Player.h"
#include "Content/Behaviours/DivingBehaviour.h"

#include "mge\core\Physics\CollisionBehaviour.h"
#include "mge\core\Physics\PhysicsWorld.h"
#include "mge\behaviours\RigidBody.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
TestScene::TestScene() :AbstractGame(), _hud(0) {
}

void TestScene::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

//build the game _world
void TestScene::_initializeScene() {
	//_renderer->setClearColor(1, 0, 0);
	Texture* fog = Texture::load(config::MGE_TEXTURE_PATH + "fog.png");
	AbstractGame::_setFogGradient(fog);

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 0, 0), glm::perspective(glm::radians(80.0f),(16.0f/9.0f),0.1f,100000.0f));
	camera->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	//_world->add(camera);
	_world->setMainCamera(camera);

	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");

	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "water.png"), 1000, 0, Texture::load(config::MGE_TEXTURE_PATH + "white.png"), Texture::load(config::MGE_TEXTURE_PATH + "white.png"));

	GameObject* plane = new GameObject("plane", glm::vec3(0, 1000, 0));
	plane->scale(glm::vec3(100000, 100000, 100000 ));
	plane->rotate(glm::radians(180.f), glm::vec3(1, 0, 0)); 
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(textureMaterial);
	_world->add(plane);

	//FishTank* fishTank = new FishTank(glm::vec3(0,200,0), _world, "", 100, 200);
	//fishTank->setMesh(smallFish);
	//AbstractMaterial * gpuinstancing = new GPUinstancingMaterial(*fishTank->allFish);
	//fishTank->setMaterial(gpuinstancing);
	//_world->add(fishTank);
	

	LuaParser * luaparser = new LuaParser(_world);
	luaparser->loadFile((config::MGE_LEVEL_PATH + "sceneWithFish.lua").c_str());

	
	GameObject* test = new GameObject("", glm::vec3(0, 100, -70));
	GameObject* playerDivingAnimationContainer = new GameObject("");
	Player* player = new Player();
	test->add(playerDivingAnimationContainer);
	_world->add(test);
	//_world->add(camera);
	playerDivingAnimationContainer->add(player);
	playerDivingAnimationContainer->addBehaviour(new DivingAnimationBehaviour());
	player->addBehaviour(new DivingBehaviour());
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(player->getBulletPhysicsTransform());

	RigidBody& playerRigidbody = playerDivingAnimationContainer->addCollider(SphereColliderArgs(3), false).makeRigidBody(1, btVector3(), *fallMotionState);
	//RigidBody& rigidbody = playerDivingAnimationContainer->addRigidBody(1, btVector3(), *fallMotionState);
	player->add(camera);

	Light* light3 = new Light(Light::lightType::DIRECTIONAL, "light3", glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1), 1, glm::vec3(), 45);
	_world->add(light3);
}

void TestScene::_render() {
	AbstractGame::_render();
	//_world->debugDraw();
	_updateHud();
	AbstractGame::_renderToQuad();
}

void TestScene::_updateHud() {
	string debugInfo = "";
	debugInfo += string("FPS:") + std::to_string((int)_fps) + "\n";

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

void TestScene::onCollisionRemoveSelf(OnCollisionArgs onCollisionArgs)
{
	std::cout << "SHIT HAPPENED" << std::endl;
	_world->physics->removeCollisionObject(onCollisionArgs.sender);
	_world->remove(dynamic_cast<AbstractBehaviour*>(onCollisionArgs.sender)->getOwner());
	delete onCollisionArgs.sender;
	std::cout << "TEAPOT COLLIDING WITH COLLISION OBJECT" << std::endl;
}

void TestScene::onCollisionRemoveOther(OnCollisionArgs onCollisionArgs)
{
	std::cout << "SHIT HAPPENED" << std::endl;
	_world->physics->removeCollisionObject(onCollisionArgs.collidingWith);
	_world->remove(dynamic_cast<AbstractBehaviour*>(onCollisionArgs.collidingWith)->getOwner());
	delete onCollisionArgs.collidingWith;
}

TestScene::~TestScene() {
	//dtor
}