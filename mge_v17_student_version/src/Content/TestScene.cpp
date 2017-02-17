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
	Camera* camera = new Camera("camera", glm::vec3(0, 0, 0));
	camera->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	//_world->add(camera);
	_world->setMainCamera(camera);

	//MESHES

	Mesh* smallFish = Mesh::load(config::MGE_MODEL_PATH + "fishLP.obj");
	//load a bunch of meshes we will be using throughout this demo
	//each mesh only has to be loaded once, but can be used multiple times:
	//F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	Mesh* suzannaMeshF = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");
	Mesh* teapotMeshS = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");
	Mesh* ship = Mesh::load(config::MGE_MODEL_PATH + "MeshScale.obj");
	// Mesh* carMesh = Mesh::load(config::MGE_MODEL_PATH+"car.obj");
	//MATERIALS

	AbstractMaterial* colorMaterial = new WobbleMaterial(Texture::load(config::MGE_TEXTURE_PATH + "color.jpg"));

	//0 specular ground material
	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "grass_texture.jpg"), 10, 1, Texture::load(config::MGE_TEXTURE_PATH + "Missing.jpg"));

	//10 specular teapot material
	AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), 1, 1);
	AbstractMaterial* waveMaterial = new LitWaveMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "CreatureUV_GRN.png"), 1, 1);

	//SCENE SETUP
	//PhysicsObject* plane = new PhysicsObject("plane", glm::vec3(0, 0, 0));
	//plane->scale(glm::vec3(50, 50, 50));
	//plane->setMesh(planeMeshDefault);
	//plane->setMaterial(waveMaterial);
	//plane->addBehaviour(new KeysBehaviour());

	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	////groundShape->setLocalScaling(btVector3(50,50,50));

	//btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	//btRigidBody::btRigidBodyConstructionInfo
	//	groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	//plane->rigidBody = new btRigidBody(groundRigidBodyCI);
	//_world->add(plane);
	//World::addRigidBody(plane->rigidBody);




	//FishTank* fishTank = new FishTank(glm::vec3(0,200,0), _world, "", 100, 200);
	//fishTank->setMesh(smallFish);
	//AbstractMaterial * gpuinstancing = new GPUinstancingMaterial(*fishTank->allFish);
	//fishTank->setMaterial(gpuinstancing);
	//_world->add(fishTank);
	

	LuaParser * luaparser = new LuaParser(_world);
	luaparser->loadFile((config::MGE_LEVEL_PATH + "sceneWithFish.lua").c_str());

	

	/*GameObject* shipGO = new GameObject("ship", glm::vec3(3, 1, 0));
	Trigger& randomTrigger = *new Trigger(*World::physics, ship->getMeshCollisionShape());

	shipGO->setMesh(ship);
	shipGO->setMaterial(textureMaterial2);
	float scale = 4;
	shipGO->scale(glm::vec3(scale, scale, scale));
	shipGO->rotate(glm::radians(90.0), glm::vec3(0, 1, 0));
	shipGO->addBehaviour(&randomTrigger);
	//teapotTrigger.collisionEvents[&randomTrigger].bind(this, &TestScene::onTeapotCollisionWithPhysicsObject);
	_world->add(shipGO);*/

	//    for(int i = 0; i < 1000; i++){
	//    GameObject* teapot2 = new GameObject ("teapot", glm::vec3(-3,1,0));
	//    teapot2->setMesh (teapotMeshS);
	//    teapot2->setMaterial(textureMaterial2);
	//    teapot2->setBehaviour (new KeysBehaviour());
	//    _world->add(teapot2);
	//    }

	//     GameObject* car = new GameObject ("car", glm::vec3(0,1,0));
	//    car->setMesh (carMesh);
	//    car->setMaterial(colorMaterial);
	//    _world->add(car);
	
	GameObject* test = new GameObject("", glm::vec3(702.763, 718.598, -39.4018));
	GameObject* playerDivingAnimationContainer = new GameObject("");
	Player* player = new Player();
	test->add(playerDivingAnimationContainer);
	_world->add(test);
	//_world->add(camera);
	playerDivingAnimationContainer->add(player);
	playerDivingAnimationContainer->addBehaviour(new DivingAnimationBehaviour());
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(player->getBulletPhysicsTransform());

	RigidBody& playerRigidbody = playerDivingAnimationContainer->addCollider(SphereColliderArgs(3), false).makeRigidBody(1, btVector3(), *fallMotionState);
	//RigidBody& rigidbody = playerDivingAnimationContainer->addRigidBody(1, btVector3(), *fallMotionState);
	player->add(camera);

	Collider& playerTriggerCollider = player->addCollider(SphereColliderArgs(5), true);
	
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 2; j++) {
			GameObject* monkey = new GameObject("monkey", glm::vec3(3, 100, 0));
			float mass = 1;
			btVector3 fallInertia(0, 0, 0);
			btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-13 + i*1.5f, 150 + i, 0 + j * 3)));
			Collider& monkeyCollider = monkey->addCollider(SphereColliderArgs(1), false);
			RigidBody& monkeyRigidbody = monkeyCollider.makeRigidBody(mass, fallInertia, *fallMotionState);
			//RigidBody& rigidBody = monkey->addRigidBody(mass, fallInertia, *fallMotionState);
			//monkey->addCollider<BoxCollider>(glm::vec4(10, 10, 10, 10));
			monkey->setMesh(suzannaMeshF);
			float scale = 1;
			monkey->scale(glm::vec3(scale, scale, scale));
			monkey->setMaterial(textureMaterial2);
			_world->add(monkey);

			//---playerTriggerCollider has been created by using:---\\
			//Collider& playerTriggerCollider = player->addCollider(SphereColliderArgs(5), true);	(where true states that it a trigger)

			//---monkeyRigidbody has been created by using:---\\
			//Collider& monkeyCollider = monkey->addCollider(SphereColliderArgs(1), false);		(where false states that it's not a trigger)
			//RigidBody& monkeyRigidbody = monkeyCollider.makeRigidBody(mass, fallInertia, *fallMotionState);

			playerTriggerCollider.collisionEnterEvents[&monkeyRigidbody].bind(this, &TestScene::onCollisionRemoveOther);
		}
	}

	AbstractMaterial* relicAndTreasureMaterial = new ColorMaterial(glm::vec3(10, 7, 0.5));

	std::vector<glm::vec3> relicLocations
	{
		glm::vec3(1313.73, 318.135, -104.237),
		glm::vec3(1331.22, 332.901, 5.06077),
		glm::vec3(1208.6, 269.945, 60.0267)
	};
	Mesh* relicMesh = Mesh::load(config::MGE_MODEL_PATH + "relic_alienTablet.obj");
	Mesh* relicMesh2 = Mesh::load(config::MGE_MODEL_PATH + "relic_disc.obj");
	for (int i=0;i<relicLocations.size();i++)
	{
		glm::vec3 relicLocation = relicLocations[i];
		GameObject* teapot = new GameObject("Relic", relicLocation);
		teapot->setMesh(i%2==0?relicMesh:relicMesh2);
		teapot->setMaterial(relicAndTreasureMaterial);
		teapot->scale(glm::vec3(0.3, 0.3, 0.3));
		teapot->addBehaviour(new RotatingBehaviour());
		Collider& teapotTriggerCollider = teapot->addCollider(CapsuleColliderArgs(1, 2), true);
		_world->add(teapot);
		teapotTriggerCollider.collisionEnterEvents[&playerRigidbody].bind(this, &TestScene::onCollisionRemoveSelf);
	}
	std::vector<glm::vec3> treasureLocations
	{
		glm::vec3(1260.71, 504.485, 43.6736)
	};
	Mesh* treasureMesh = Mesh::load(config::MGE_MODEL_PATH + "TreasureChest.obj");
	for each (glm::vec3 treasureLocation in treasureLocations)
	{
		GameObject* teapot = new GameObject("Treasure", treasureLocation);
		teapot->setMesh(treasureMesh);
		teapot->setMaterial(relicAndTreasureMaterial);
		teapot->addBehaviour(new RotatingBehaviour());
		teapot->scale(glm::vec3(1.5,1.5,1.5));
		Collider& teapotTriggerCollider = teapot->addCollider(CapsuleColliderArgs(1, 2), true);
		_world->add(teapot);
		teapotTriggerCollider.collisionEnterEvents[&playerRigidbody].bind(this, &TestScene::onCollisionRemoveSelf);
	}

	//bool isTrigger = true;
	//Collider& collider = player->addCollider(CapsuleColliderArgs(1,2), isTrigger);
	//collider->makeRigidbody(1);

	//if collider is trigger, shouldn't be updated by physics.
	//if collider is trigger, can't be a rigidbody
	//if collider is solid. Can be a rigidbody, doesn't have to be.
	
	
	
	
	
	/*
	GameObject* test = new GameObject("", glm::vec3(0, 100, -70));
	GameObject* playerDivingAnimationContainer = new GameObject("");
	Player* player = new Player();
	test->add(playerDivingAnimationContainer);
	_world->add(test);
	playerDivingAnimationContainer->add(player);
	playerDivingAnimationContainer->addBehaviour(new DivingAnimationBehaviour());
	player->addCollider(SphereColliderArgs(1));
	//player->addRigidBody(1);
	player->add(camera);*/

	//camera->addBehaviour(new CameraOrbitBehaviour (10, 30, 150, 1, teapot));
	//
	//    glm::vec3* lightColor = new glm::vec3(0.5f,0.0f,.5f);
	//    Light* light = new Light (Light::lightType::POINT, "light1", glm::vec3(0,2,-5), *lightColor, 50, glm::vec3(0,0,1));
	//    light->setMesh (cubeMeshF);
	//    AbstractMaterial* colorMaterial2 = new ColorMaterial (*lightColor);
	//    light->setBehaviour(new LookAt(teapot));
	//    light->setMaterial(colorMaterial2);
	//    _world->add(light);
	//
	//    glm::vec3* lightColor2 = new glm::vec3(0.0f,0.0f,.5f);
	//    Light* light2 = new Light (Light::lightType::POINT, "light1", glm::vec3(0,2,5), *lightColor2, 50, glm::vec3(0,0,1));
	//    light2->setMesh (cubeMeshF);
	//    light->setBehaviour(new LookAt(teapot));
	//    light2->setMaterial(colorMaterial2);
	//    _world->add(light2);
	float random = time(NULL);
	std::cout << "random seed: " << random << std::endl;
	std::srand(random);
	for (int i = 0; i < 3; i++) {
		glm::vec3* lightColor = new glm::vec3(rand() % 100, rand() % 100, rand() % 100);
		Light* light = new Light(Light::lightType::POINT, "light1", glm::vec3(rand() % 100 - 50, 5, rand() % 100 - 50), *lightColor, 50, glm::vec3(0, 0, 1));
		//light->setMesh (cubeMeshF);
		AbstractMaterial* colorMaterial2 = new ColorMaterial(glm::normalize(*lightColor));
		//light->setBehaviour(new LookAt(teapot));
		//light->setMaterial(colorMaterial2);
		_world->add(light);
	}

	Light* light3 = new Light(Light::lightType::DIRECTIONAL, "light3", glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1), 1, glm::vec3(), 45);
	//light3->setMesh (cubeMeshF);
	//light3->setMaterial(new ColorMaterial(glm::normalize(glm::vec3(1, 1, 1))));
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