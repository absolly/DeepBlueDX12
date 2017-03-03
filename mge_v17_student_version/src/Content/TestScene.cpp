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
#include "mge/core/SoundManager.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/WobbleMaterial.hpp"
#include "mge/materials/SeaMaterial.hpp"
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
	_hud = new Hud(_window);
	cout << "HUD initialized." << endl << endl;
}

//build the game _world
void TestScene::_initializeScene() {
	//_renderer->setClearColor(1, 0, 0);
	Texture* fog = Texture::load(Config::MGE_TEXTURE_PATH + "fog.png");
	AbstractGame::_setFogGradient(fog);

	//add camera first (it will be updated last)
	//Camera* camera = new Camera("camera", glm::vec3(0, 0, 0), glm::perspective(glm::radians(80.0f),(16.0f/9.0f),.5f,100000.0f));
	//camera->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	//_world->add(camera);
	//_world->setMainCamera(camera);

	Mesh* planeMeshDefault = Mesh::load(Config::MGE_MODEL_PATH + "plane_8192.obj");

	AbstractMaterial* templeMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"), 1000);
	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "water.png"), 1000, 0, Texture::load(Config::MGE_TEXTURE_PATH + "white.png"), Texture::load(Config::MGE_TEXTURE_PATH + "white.png"));
	AbstractMaterial* textureMaterial2 = new SeaMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "seatexture.jpg"), 1);

	/*GameObject* plane = new GameObject("plane", glm::vec3(0, 727.386, 0));
	plane->scale(glm::vec3(500, 500, 500 ));
	plane->rotate(glm::radians(180.f), glm::vec3(1, 0, 0)); 
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(textureMaterial);
	_world->add(plane);
*/
	Mesh * smallFish = Mesh::load(Config::MGE_MODEL_PATH + "fishLP.obj");

	FishTank* fishTank = new FishTank(glm::vec3(-1200,300,270), _world, "", 150, 200, 20);
	fishTank->setMesh(smallFish);
	AbstractMaterial * gpuinstancing = new GPUinstancingMaterial(*fishTank->allFish);
	fishTank->setMaterial(gpuinstancing);
	_world->add(fishTank);

	Player* player = new Player();
	_world->add(player);
	_world->setMainCamera(player->getCamera());
	RigidBody* playerRigidbody = player->getBehaviour<RigidBody>();

	/*
	GameObject* player = new GameObject("player", glm::vec3(-2000, 718.598, -700));
	Player* playerContainer = new Player();

	GameObject* playerDivingAnimationContainer = new GameObject("playerDivingAnimationContainer");
	player->add(playerDivingAnimationContainer);
	_world->add(player);
	playerDivingAnimationContainer->add(playerContainer);
	playerDivingAnimationContainer->addBehaviour(new DivingAnimationBehaviour());
	DivingBehaviour* divingBehaviour = new DivingBehaviour();
	playerContainer->addBehaviour(divingBehaviour);
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(playerContainer->getBulletPhysicsTransform());

	RigidBody& playerRigidbody = playerDivingAnimationContainer->addCollider(SphereColliderArgs(3), false, false).makeRigidBody(1, btVector3(), *fallMotionState);
	//RigidBody& rigidbody = playerDivingAnimationContainer->addRigidBody(1, btVector3(), *fallMotionState);
	playerContainer->add(camera);
	*/



	//LuaParser * luaparser = new LuaParser(_world);
//	luaparser->loadFile((Config::MGE_LEVEL_PATH + "sceneWithFish.lua").c_str());

	//ADDING BOAT FOLLOWING PLAYER
	Mesh* boatMesh = Mesh::load(Config::MGE_MODEL_PATH + "boat_baseTank9.obj");
	GameObject* boat = new GameObject("Boat", glm::vec3(0,720,0));
	boat->setMesh(boatMesh);
	boat->setMaterial(textureMaterial);
	//boat->scale(glm::vec3(0.1));
	//boat->scale(glm::vec3(0.05f, 0.05f, 0.05f));
	//float surfaceHeight = 750;
	boat->addBehaviour(new BoatFollowBehaviour(player->getChildAt(0)));
	//boat->addCollider(MeshColliderArgs(*boatMesh), false, false);
	GameObject* boatColliderContainer = new GameObject("ColliderContainer", glm::vec3(0,50,0));
	boat->add(boatColliderContainer);
	Collider& boatTriggerCollider = boatColliderContainer->addCollider(CapsuleColliderArgs(15,50), true, false);
	boatColliderContainer->rotate(glm::radians(90.0f), glm::vec3(1,0, 0));
	
	boatTriggerCollider.collisionEvents[playerRigidbody].bind(player->getBehaviour<DivingBehaviour>(), &DivingBehaviour::onCollisionAddAir);
	
	_world->add(boat);

	//ADDING BOAT FOLLOWING PLAYER

	GameObject * objtest1 = new GameObject("name", glm::vec3(-2078, 290, 766));
	GameObject * objtest2 = new GameObject("name", glm::vec3(-2292, 22, 2093));
	GameObject * objtest3 = new GameObject("name", glm::vec3(304, 116, 297));

	std::vector<GameObject*> * objectives = new std::vector<GameObject*>();
	objectives->push_back(objtest1);
	objectives->push_back(objtest2);
	objectives->push_back(objtest3);

	SoundManager * soundmng = new SoundManager();

	_scriptParser = new LuaScriptParser((Config::MGE_LEVEL_PATH + "story.lua").c_str(), _window, soundmng);
	_scriptParser->SetPlayerAndObjectives(player->getChildAt(0), objectives);

	LuaParser * luaparser2 = new LuaParser(_world);
	luaparser2->setPlayerRigidBody(*playerRigidbody);
	luaparser2->scriptParser = _scriptParser;
	luaparser2->loadFile((Config::MGE_LEVEL_PATH + "playTestLua.lua").c_str());

	AbstractMaterial* relicAndTreasureMaterial = new ColorMaterial(glm::vec3(10, 7, 0.5));
	std::vector<glm::vec3> relicLocations
	{
		glm::vec3(387.028, 189.747, -283.077),
		glm::vec3(414.763, 144.757, -296.977),
		glm::vec3(306.804, 118.942, -82.5193)
	};

	std::string relicNames[] =
	{
		"relic_tablet",
		"relic_disc",
		"relic_statue"
	};
	for (int i = 0; i<relicLocations.size(); i++)
	{
		glm::vec3 relicLocation = relicLocations[i];
		GameObject* teapot = new GameObject(relicNames[i], relicLocation);
		teapot->setMesh(Mesh::load(Config::MGE_MODEL_PATH + relicNames[i] + ".obj"));
		teapot->setMaterial(relicAndTreasureMaterial);
		teapot->scale(glm::vec3(0.3, 0.3, 0.3));
		Collider& teapotTriggerCollider = teapot->addCollider(CapsuleColliderArgs(4, 8), true, true);
		_world->add(teapot);
		teapotTriggerCollider.collisionEvents[playerRigidbody].bind(this, &TestScene::onCollisionRemoveSelf);
	}

	Mesh* templeMesh = Mesh::load(Config::MGE_MODEL_PATH + "TempleWODoors.obj");
	Mesh* templeDoorsMesh = Mesh::load(Config::MGE_MODEL_PATH + "TempleDoors.obj");


	GameObject* temple = new GameObject("Temple");
	GameObject* templeDoors = new GameObject("TempleDoors");

	temple->setMesh(templeMesh);
	templeDoors->setMesh(templeDoorsMesh);

	temple->setMaterial(templeMaterial);
	templeDoors->setMaterial(templeMaterial);

	temple->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	templeDoors->scale(glm::vec3(0.1f, 0.1f, 0.1f));

	temple->setLocalPosition(glm::vec3(-402.35, 79.9474, 995.567));
	templeDoors->setLocalPosition(glm::vec3(-404.713, 82.5058, 1362.81));

	Collider& templeDoorsCollider = templeDoors->addCollider(BoxColliderArgs(200, 800, 20), true, true);
	templeDoorsCollider.collisionEnterEvents[playerRigidbody].bind(this, &TestScene::onTempleDoorCollision);

	_world->add(temple);
	_world->add(templeDoors);

	/*std::vector<glm::vec3> treasureLocations
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
	}*/

	Light* light3 = new Light(Light::lightType::DIRECTIONAL, "light3", glm::vec3(500, 0, 500), glm::vec3(0.1, 0.1, 0.1), 1, glm::vec3());
	light3->rotate(glm::radians(-75.f), glm::vec3(1, 0.05f, 0));
	light3->addBehaviour(new CopyTargetPositionBehaviour(player->getChildAt(0)));
	_world->add(light3);



	//SEA COLLIDER
	btCollisionShape* collisionShape = new btStaticPlaneShape(btVector3(0, -1, 0), 1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody* rigidBody = new btRigidBody(0, groundMotionState, collisionShape, btVector3(0, 0, 0));
	btTransform transform = rigidBody->getCenterOfMassTransform();
	transform.setOrigin(btVector3(0, 720, 0));
	rigidBody->setCenterOfMassTransform(transform);
	_world->physics->addRigidBody(rigidBody);
	//SEA COLLIDER

	GameObject* sea = new GameObject("sea", glm::vec3(0, 720, 0));
	sea->setMesh(planeMeshDefault);
	sea->setMaterial(textureMaterial2);
	sea->scale(glm::vec3(5000, 1, 5000));
	sea->addBehaviour(new CopyTargetPositionBehaviour(player->getChildAt(0), glm::vec3(1, 0, 1)));
	sea->rotate(glm::radians(180.f), glm::vec3(1, 0, 0));
	_world->add(sea);
}

void TestScene::_render() {
	AbstractGame::_render();
	_world->debugDraw();
	//_world->debugDraw();

	AbstractGame::_renderToQuad();
	_updateHud();

	_scriptParser->step();

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
	AbstractBehaviour* abstractBehaviour = dynamic_cast<AbstractBehaviour*>(onCollisionArgs.sender);
	Hud::getInstance()->getInventory().addItem(abstractBehaviour->getOwner()->getName() + ".png");
	std::cout << "SHIT HAPPENED" << std::endl;
	_world->physics->removeCollisionObject(onCollisionArgs.sender);
	_world->remove(abstractBehaviour->getOwner());
	delete onCollisionArgs.sender;
	std::cout << "TEAPOT COLLIDING WITH COLLISION OBJECT" << std::endl;
}

void TestScene::onTempleDoorCollision(OnCollisionArgs onCollisionArgs)
{
	int itemIndexOfRelicStatue = Hud::getInstance()->getInventory().getItemIndex("relic_statue.png");
	if (itemIndexOfRelicStatue == -1)
	{
		std::cout << "YOU NEED A RELIC STATUE TO OPEN THIS DOOR";
		return;
	}
	std::cout << "You used a relic statue to open the door.";
	Hud::getInstance()->getInventory().removeItem(itemIndexOfRelicStatue);

	AbstractBehaviour* abstractBehaviour = dynamic_cast<AbstractBehaviour*>(onCollisionArgs.sender);
	_world->physics->removeCollisionObject(onCollisionArgs.sender);
	_world->remove(abstractBehaviour->getOwner());
	delete onCollisionArgs.sender;
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