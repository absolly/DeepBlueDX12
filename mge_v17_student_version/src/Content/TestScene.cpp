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
#include "mge/materials/LitWaveMaterial.hpp"
#include "mge/materials/SeaMaterial.hpp"
#include "mge/materials/GPUinstancingMaterial.hpp"
#include "mge/materials/BillBoardMaterial.hpp"

#include "Content/GameObjects/ParticleSystem.hpp"
#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/CameraOrbitBehaviour.hpp"
#include "mge/behaviours/CopyTargetPositionBehaviour.hpp"
#include "mge/behaviours/BoatFollowingBehaviour.hpp"
#include "Content\Behaviours\Player\DivingAnimationBehaviour.h"
#include "Content/Behaviours/BoatFollowBehaviour.h"
#include "Content/Behaviours/Player/PlayerFishAbilityBehaviour.h"
#include "mge/util/DebugHud.hpp"

#include "Content/GameObjects/FishTank.hpp"
#include "Content/GameObjects/PlayerFishFlock.hpp"

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
	_renderer->setClearColor(0, 0, 0);
	Texture* fog = Texture::load(Config::MGE_TEXTURE_PATH + "fog2.png");
	AbstractGame::_setFogGradient(fog);




	//add camera first (it will be updated last)
	//Camera* camera = new Camera("camera", glm::vec3(0, 0, 0), glm::perspective(glm::radians(80.0f),(16.0f/9.0f),.5f,100000.0f));
	//camera->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	//_world->add(camera);
	//_world->setMainCamera(camera);

	Mesh* planeMeshDefault = Mesh::load(Config::MGE_MODEL_PATH + "plane_8192.obj");

	AbstractMaterial* templeMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"), 1000);
	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "missing.jpg"), 1, 0, Texture::load(Config::MGE_TEXTURE_PATH + "Black.png"), Texture::load(Config::MGE_TEXTURE_PATH + "NormalNormalMap.png"));
	AbstractMaterial* textureMaterial2 = new SeaMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "seanormal.jpg"), 1);
	/*GameObject* plane = new GameObject("plane", glm::vec3(0, 727.386, 0));
	plane->scale(glm::vec3(500, 500, 500 ));
	plane->rotate(glm::radians(180.f), glm::vec3(1, 0, 0));
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(textureMaterial);
	_world->add(plane);
*/
	Mesh * smallFish = Mesh::load(Config::MGE_MODEL_PATH + "fishLP.obj");

	Player* player = new Player();
	AbstractBehaviour * playerap = new PlayerFishAbilityBehaviour(_world, player->getChildAt(0));
	player->getChildAt(0)->addBehaviour(playerap);
	_world->add(player);
	_world->setMainCamera(player->getCamera());
	RigidBody* playerRigidbody = player->getChildAt(0)->getBehaviour<RigidBody>();

	glm::vec3 ParticlePosition = player->getChildAt(0)->getWorldPosition();
	ParticlePosition.y -= 20;
	ParticleSystem * particleSystem = new ParticleSystem(ParticlePosition, "name");
	particleSystem->SetStartEndScale(0.001f, 1.0f);

	particleSystem->setMesh(planeMeshDefault);
	Texture* bubble = Texture::load(Config::MGE_TEXTURE_PATH + "bubble.png");
	BillBoardMaterial * billboardMat = new BillBoardMaterial(particleSystem, bubble);
	particleSystem->setMaterial(billboardMat);
	_world->add(particleSystem);

	ParticlePosition += 2;

	//ParticleSystem * particleSystem2 = new ParticleSystem(ParticlePosition, "name");

	//particleSystem2->setMesh(planeMeshDefault);
	//BillBoardMaterial * billboardMat2 = new BillBoardMaterial(particleSystem2, bubble);
	//particleSystem2->setMaterial(billboardMat2);
	//_world->add(particleSystem2);
	//ParticlePosition.z += 10;

	//for (int i = 0; i < 5; i++)
	//{
	//	ParticlePosition.x += 1;
	//	FishTank * tank = new FishTank(ParticlePosition, _world, "fishies", 50, 100, 15);
	//	tank->setMesh(smallFish);
	//	tank->setMaterial(new GPUinstancingMaterial(*tank->allFish));
	//	_world->add(tank);
	//}



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
	GameObject* boat = new GameObject("Boat", glm::vec3(-2068, 720, 541));
	boat->setMesh(boatMesh);
	boat->setMaterial(textureMaterial);
	//boat->scale(glm::vec3(0.1));
	//boat->scale(glm::vec3(0.05f, 0.05f, 0.05f));
	//float surfaceHeight = 750;
	boat->addBehaviour(new BoatFollowBehaviour(player->getChildAt(0)));
	//boat->addCollider(MeshColliderArgs(*boatMesh), false, false);
	GameObject* boatColliderContainer = new GameObject("ColliderContainer", glm::vec3(0, -80, 0));
	boat->add(boatColliderContainer);
	Collider& boatTriggerCollider = boatColliderContainer->addCollider(CapsuleColliderArgs(15, 60), true, false);
	boatColliderContainer->rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));

	boatTriggerCollider.collisionEvents[playerRigidbody].bind(player->getBehaviour<DivingBehaviour>(), &DivingBehaviour::onCollisionAddAir);
	boatTriggerCollider.collisionExitEvents[playerRigidbody].bind(player->getBehaviour<DivingBehaviour>(), &DivingBehaviour::onExitCollisionAddAir);

	_world->add(boat);

	//ADDING BOAT FOLLOWING PLAYER


	//GameObject* teapot = new GameObject("teapot", glm::vec3(-2068, 283, 541));
	//glm::vec3 _waypoints[10]{};


	//float random = 9;
	//std::cout << "random seed: " << random << std::endl;
	//srand(random);
	//Mesh* cubeMeshF = Mesh::load(Config::MGE_MODEL_PATH + "cube_flat.obj");
	//for (int i = 0; i < 10; i++) {
	//	glm::vec3* lightColor = new glm::vec3(rand() % 100, rand() % 100, rand() % 100);
	//	glm::vec3 pos = glm::vec3((rand() % 100 - 50) - 2068, 283, (rand() % 100 - 50)+541);
	//	GameObject* light = new GameObject("light1", pos);
	//	_waypoints[i] = pos;
	//	light->setMesh(cubeMeshF);
	//	AbstractMaterial* colorMaterial2 = new ColorMaterial((*lightColor));
	//	//light->setBehaviour(new LookAt(teapot));
	//	light->setMaterial(colorMaterial2);
	//	_world->add(light);
	//}
	//Mesh* teapotMeshS = Mesh::load(Config::MGE_MODEL_PATH + "MantaRay.obj");
	//AbstractMaterial* waveMaterial = new LitWaveMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "RayAnimUV.png"), 1, 1, Texture::load(Config::MGE_TEXTURE_PATH + "black.png"), Texture::load(Config::MGE_TEXTURE_PATH + "BricksNormal.png"));

	//teapot->addBehaviour(new PredatorBehaviour(player->getChildAt(0), _waypoints, _world));
	//teapot->setMesh(teapotMeshS);
	//teapot->setMaterial(waveMaterial);
	//_world->add(teapot);
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
	luaparser2->setPlayerRigidBody(*player);
	luaparser2->scriptParser = _scriptParser;
	luaparser2->loadFile((Config::MGE_LEVEL_PATH + "playTestLua.lua").c_str());

	AbstractMaterial* relicAndTreasureMaterial = new ColorMaterial(glm::vec3(10, 7, 0.5));
	std::vector<glm::vec3> relicLocations
	{
		glm::vec3(-2238.6, +12.7, +2198.9), //In Tutorial cave
		glm::vec3(414.763, 144.757, -296.977), 
		glm::vec3(306.804, 118.942, -82.5193)
	};

	std::vector<glm::vec3> relicScales
	{	
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(0.3f, 0.3f, 0.3f)
	};

	std::string relicNames[] =
	{
		"relic_tablet",
		"relic_disc",
		"relic_statue"
	};
	for (int i = 0; i < relicLocations.size(); i++)
	{
		glm::vec3 relicLocation = relicLocations[i];
		GameObject* teapot = new GameObject(relicNames[i], relicLocation);
		teapot->setMesh(Mesh::load(Config::MGE_MODEL_PATH + relicNames[i] + ".obj"));
		teapot->setMaterial(relicAndTreasureMaterial);
		teapot->scale(relicScales[i]);
		teapot->addBehaviour(new RotatingBehaviour());
		Collider& teapotTriggerCollider = teapot->addCollider(CapsuleColliderArgs(12, 16), true, true);
		_world->add(teapot);
		teapotTriggerCollider.collisionEvents[playerRigidbody].bind(this, &TestScene::onCollisionRemoveSelf);
	}

	/*
	Mesh* templeMesh = Mesh::load(Config::MGE_MODEL_PATH + "TempleWODoors.obj");
	Mesh* templeDoorsMesh = Mesh::load(Config::MGE_MODEL_PATH + "TempleDoors.obj");

	GameObject* temple = new GameObject("Temple");
	GameObject* templeDoors = new GameObject("TempleDoors");

	emple->setMesh(templeMesh);
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
	*/

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

	Light* light3 = new Light(Light::lightType::DIRECTIONAL, "light3", glm::vec3(500, 0, 500), glm::vec3(0.905882, 0.807843, 0.572549), 700, glm::vec3(0, 0, 1));
	light3->rotate(glm::radians(-75.f), glm::vec3(1, 0.05f, 0));
	light3->addBehaviour(new CopyTargetPositionBehaviour(player->getChildAt(0)));
	_world->add(light3);


	GameObject* seaCollider = new GameObject("SeaCollider", glm::vec3(0, 720, 0));
	seaCollider->addBehaviour(new CopyTargetPositionBehaviour(player->getChildAt(0), glm::bvec3(true, false, true)));
	seaCollider->addCollider(BoxColliderArgs(100, 5, 100), false, false);
	_world->add(seaCollider);
	//SEA COLLIDER
	//btCollisionShape* collisionShape = new btStaticPlaneShape(btVector3(0, -1, 0), 1);
	//btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0), btVector3(0, 720, 0)));
	//btRigidBody* rigidBody = new btRigidBody(0, groundMotionState, collisionShape, btVector3(0, 0, 0));
	//btTransform transform = rigidBody->getCenterOfMassTransform();
	//transform.setOrigin(btVector3(0, 720, 0));
	//rigidBody->setCenterOfMassTransform(transform);
	//_world->physics->addRigidBody(rigidBody);
	//SEA COLLIDER

	GameObject* sea = new GameObject("sea", glm::vec3(0, 720, 0));
	sea->setMesh(planeMeshDefault);
	sea->setMaterial(textureMaterial2);
	sea->scale(glm::vec3(5000, 1, 5000));
	sea->addBehaviour(new CopyTargetPositionBehaviour(player->getChildAt(0), glm::bvec3(true, false, true)));
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