#include <glm.hpp>
#include <iostream>
#include <string>

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
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/CameraOrbitBehaviour.hpp"
#include "mge/behaviours/CopyTargetPositionBehaviour.hpp"
#include "mge/behaviours/BoatFollowingBehaviour.hpp"
#include "Content\Behaviours\Player\DivingAnimationBehaviour.h"
#include "Content/Behaviours/BoatFollowBehaviour.h"
#include "mge/util/DebugHud.hpp"

#include "Content/GameObjects/FishTank.hpp"

#include "mge/config.hpp"
#include "Content/TestScene.hpp"
#include <time.h>       /* time */
#include "Content\GameObjects\Player.h"
#include "Content/Behaviours/DivingBehaviour.h"

#include "mge\core\Physics\CollisionBehaviour.h"
#include "mge\core\Physics\PhysicsWorld.h"
#include "mge\core\Physics\RigidBody.hpp"

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
	Texture* fog = Texture::load(Config::MGE_TEXTURE_PATH + "fog.png");
	AbstractGame::_setFogGradient(fog);

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 0, 0), glm::perspective(glm::radians(80.0f),(16.0f/9.0f),0.1f,100000.0f));
	camera->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	//_world->add(camera);
	_world->setMainCamera(camera);

	Mesh* planeMeshDefault = Mesh::load(Config::MGE_MODEL_PATH + "plane.obj");

	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "water.png"), 1000, 0, Texture::load(Config::MGE_TEXTURE_PATH + "white.png"), Texture::load(Config::MGE_TEXTURE_PATH + "white.png"));

	/*GameObject* plane = new GameObject("plane", glm::vec3(0, 727.386, 0));
	plane->scale(glm::vec3(500, 500, 500 ));
	plane->rotate(glm::radians(180.f), glm::vec3(1, 0, 0)); 
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(textureMaterial);
	_world->add(plane);
*/
	//FishTank* fishTank = new FishTank(glm::vec3(0,200,0), _world, "", 100, 200);
	//fishTank->setMesh(smallFish);
	//AbstractMaterial * gpuinstancing = new GPUinstancingMaterial(*fishTank->allFish);
	//fishTank->setMaterial(gpuinstancing);
	//_world->add(fishTank);
	

	LuaParser * luaparser = new LuaParser(_world);
	luaparser->loadFile((Config::MGE_LEVEL_PATH + "sceneWithFish.lua").c_str());



	GameObject* test = new GameObject("", glm::vec3(702.763, 718.598, -39.4018));
	GameObject* playerDivingAnimationContainer = new GameObject("");
	Player* player = new Player();
	test->add(playerDivingAnimationContainer);
	_world->add(test);
	//_world->add(camera);
	playerDivingAnimationContainer->add(player);
	playerDivingAnimationContainer->addBehaviour(new DivingAnimationBehaviour());
	player->addBehaviour(new DivingBehaviour());
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(player->getBulletPhysicsTransform());

	RigidBody& playerRigidbody = playerDivingAnimationContainer->addCollider(SphereColliderArgs(3), false, false).makeRigidBody(1, btVector3(), *fallMotionState);
	//RigidBody& rigidbody = playerDivingAnimationContainer->addRigidBody(1, btVector3(), *fallMotionState);
	player->add(camera);

	//ADDING BOAT FOLLOWING PLAYER
	Mesh* boatMesh = Mesh::load(Config::MGE_MODEL_PATH + "boat_baseTank9.obj");
	GameObject* boat = new GameObject("Boat", glm::vec3(0,750,0));
	boat->setMesh(boatMesh);
	boat->setMaterial(textureMaterial);
	//boat->scale(glm::vec3(0.05f, 0.05f, 0.05f));
	//float surfaceHeight = 750;
	boat->addBehaviour(new BoatFollowBehaviour(player));
	//boat->addCollider(MeshColliderArgs(*boatMesh), false, false);
	
	_world->add(boat);
	//ADDING BOAT FOLLOWING PLAYER

	AbstractMaterial* relicAndTreasureMaterial = new ColorMaterial(glm::vec3(10, 7, 0.5));
	std::vector<glm::vec3> relicLocations
	{
		glm::vec3(1313.73, 318.135, -104.237),
		glm::vec3(1331.22, 332.901, 5.06077),
		glm::vec3(1208.6, 269.945, 60.0267)
	};
	Mesh* relicMesh = Mesh::load(Config::MGE_MODEL_PATH + "relic_alienTablet.obj");
	Mesh* relicMesh2 = Mesh::load(Config::MGE_MODEL_PATH + "relic_disc.obj");
	for (int i = 0; i<relicLocations.size(); i++)
	{
		glm::vec3 relicLocation = relicLocations[i];
		GameObject* teapot = new GameObject("Relic", relicLocation);
		teapot->setMesh(i % 2 == 0 ? relicMesh : relicMesh2);
		teapot->setMaterial(relicAndTreasureMaterial);
		teapot->scale(glm::vec3(0.3, 0.3, 0.3));
		Collider& teapotTriggerCollider = teapot->addCollider(CapsuleColliderArgs(1, 2), true, true);
		_world->add(teapot);
		teapotTriggerCollider.collisionEvents[&playerRigidbody].bind(this, &TestScene::onCollisionRemoveSelf);
	}
	std::vector<glm::vec3> treasureLocations
	{
		glm::vec3(1260.71, 504.485, 43.6736)
	};
	Mesh* treasureMesh = Mesh::load(Config::MGE_MODEL_PATH + "TreasureChest.obj");
	for each (glm::vec3 treasureLocation in treasureLocations)
	{
		GameObject* teapot = new GameObject("Treasure", treasureLocation);
		teapot->setMesh(treasureMesh);
		teapot->setMaterial(relicAndTreasureMaterial);
		teapot->scale(glm::vec3(1.5, 1.5, 1.5));
		Collider& teapotTriggerCollider = teapot->addCollider(CapsuleColliderArgs(1, 2), true, true);
		_world->add(teapot);
		teapotTriggerCollider.collisionEvents[&playerRigidbody].bind(this, &TestScene::onCollisionRemoveSelf);
	}

	Light* light3 = new Light(Light::lightType::DIRECTIONAL, "light3", glm::vec3(500, 0, 500), glm::vec3(0.1, 0.1, 0.1), 1, glm::vec3());
	light3->rotate(glm::radians(-90.f), glm::vec3(1, 0, 0));
	light3->addBehaviour(new CopyTargetPositionBehaviour(player));
	_world->add(light3);
}

void TestScene::_render() {
	AbstractGame::_render();
	_world->debugDraw();
	//_world->debugDraw();

	AbstractGame::_renderToQuad();
	_updateHud();

	//AbstractGame::DrawQuad();
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