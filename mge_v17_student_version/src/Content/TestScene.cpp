#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"

#include "mge/core/Camera.hpp"

#include <vector>
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

	Mesh* smallFish = Mesh::load(config::MGE_MODEL_PATH + "small_fish.obj");
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


	LuaParser * luaparser = new LuaParser(_world);
	luaparser->loadFile((config::MGE_LEVEL_PATH + "poging21.lua").c_str());


	GameObject* test = new GameObject("", glm::vec3(0, 100, -70));
	GameObject* playerDivingAnimationContainer = new GameObject("");
	Player* player = new Player();
	test->add(playerDivingAnimationContainer);
	_world->add(test);
	//_world->add(camera);
	playerDivingAnimationContainer->add(player);
	playerDivingAnimationContainer->addBehaviour(new DivingAnimationBehaviour());
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(player->getBulletPhysicsTransform());

	playerDivingAnimationContainer->addCollider(SphereColliderArgs(3), false, true);
	playerDivingAnimationContainer->addRigidBody(1, btVector3(), *fallMotionState);
	player->add(camera);
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
	srand(random);
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
	_world->debugDraw();
	_updateHud();
	AbstractGame::_renderToQuad();
}

void TestScene::_updateHud() {
	string debugInfo = "";
	debugInfo += string("FPS:") + std::to_string((int)_fps) + "\n";

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

void TestScene::onTeapotCollisionWithPhysicsObject(btCollisionObject * collisionObject)
{
	_world->physics->removeCollisionObject(collisionObject);
	_world->remove(dynamic_cast<AbstractBehaviour*>(collisionObject)->getOwner());
	delete collisionObject;
	std::cout << "TEAPOT COLLIDING WITH COLLISION OBJECT" << std::endl;
}

TestScene::~TestScene() {
	//dtor
}
