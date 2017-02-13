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

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/RigidBody.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/CameraOrbitBehaviour.hpp"
#include "Content\Behaviours\Player\DivingAnimationBehaviour.h"
#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "Content/TestScene.hpp"
#include <time.h>       /* time */
#include "Content\GameObjects\Player.h"

#include "mge\core\Physics\Trigger.h"
#include "mge\core\Physics\PhysicsWorld.h"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
TestScene::TestScene():AbstractGame (),_hud(0) {
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
    _renderer->setClearColor(1,0,0);

    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,0,0));
    camera->rotate(glm::radians(180.0f), glm::vec3(0,1,0));
    //_world->add(camera);
    _world->setMainCamera(camera);

    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
    Mesh* suzannaMeshF = Mesh::load (config::MGE_MODEL_PATH+"suzanna_smooth.obj");
    Mesh* teapotMeshS = Mesh::load (config::MGE_MODEL_PATH+"teapot_smooth.obj");
	Mesh* ship = Mesh::load(config::MGE_MODEL_PATH + "MeshScale.obj");
    // Mesh* carMesh = Mesh::load(config::MGE_MODEL_PATH+"car.obj");
    //MATERIALS

    AbstractMaterial* colorMaterial = new WobbleMaterial (Texture::load (config::MGE_TEXTURE_PATH+"color.jpg"));

    //0 specular ground material
    AbstractMaterial* textureMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"grass_texture.jpg"), 10, 10, Texture::load(config::MGE_TEXTURE_PATH + "Missing.jpg"));

    //10 specular teapot material
    AbstractMaterial* textureMaterial2 = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"bricks.jpg"), 0.01, 10);

    //SCENE SETUP
    PhysicsObject* plane = new PhysicsObject ("plane", glm::vec3(0,0,0));
    plane->scale(glm::vec3(50,50,50));
    plane->setMesh(planeMeshDefault);
    plane->setMaterial(textureMaterial);

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    groundShape->setLocalScaling(btVector3(50,50,50));

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    plane->rigidBody = new btRigidBody(groundRigidBodyCI);
    _world->add(plane);
    World::addRigidBody(plane->rigidBody);

    GameObject* teapot = new GameObject ("teapot", glm::vec3(-3,1,0));
    teapot->setMesh (teapotMeshS);
    teapot->setMaterial(textureMaterial2);
    teapot->addBehaviour (new KeysBehaviour());
	Trigger& teapotTrigger = *new Trigger(*World::physics, new btBoxShape(btVector3(1,1,1)));
	teapot->addBehaviour(&teapotTrigger);
    _world->add(teapot);


	GameObject* shipGO = new GameObject("ship", glm::vec3(3, 1, 0));
	Trigger& randomTrigger = *new Trigger(*World::physics, ship->getMeshCollisionShape());
	
	shipGO->setMesh(ship);
	shipGO->setMaterial(textureMaterial2);
	shipGO->scale(glm::vec3(4,4,4));
	shipGO->rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));
	shipGO->addBehaviour(&randomTrigger);
	//teapotTrigger.collisionEvents[&randomTrigger].bind(this, &TestScene::onTeapotCollisionWithPhysicsObject);
	_world->add(shipGO);

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
    for(int i = 0; i < 25; i++) {
        for(int j = 0; j < 2; j++) {

				btCollisionShape* fallShape = new btSphereShape(1);
				btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0 + i, 150 + i, 0 + i)));
				btScalar mass = 1;
				btVector3 fallInertia(0, 0, 0);
				fallShape->calculateLocalInertia(mass, fallInertia);
				btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);

            GameObject* monkey = new GameObject ("monkey", glm::vec3(3,100,0));
            monkey->setMesh (suzannaMeshF);
            monkey->setMaterial(textureMaterial2);
			RigidBody* rigidBody = new RigidBody(fallRigidBodyCI);
			monkey->addBehaviour(rigidBody);
			//teapotTrigger.collisionEvents[rigidBody].bind(this, &TestScene::onTeapotCollisionWithPhysicsObject);
      //    monkey->setBehaviour (new RotatingBehaviour());

            _world->add(monkey);
       }
    }
	GameObject* test = new GameObject("", glm::vec3(0, 100, -70));
	GameObject* playerDivingAnimationContainer = new GameObject("");
	Player* player = new Player();
	test->add(playerDivingAnimationContainer);
	_world->add(test);
	playerDivingAnimationContainer->add(player);
	playerDivingAnimationContainer->addBehaviour(new DivingAnimationBehaviour());
	player->add(camera);

	

	glm::mat4 mat4 = glm::mat4(1);
	std::cout << mat4 << std::endl;
	glm::mat4 rotatedMatrix = glm::rotate(mat4, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	std::cout << rotatedMatrix << std::endl;

	
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
    std:: cout << "random seed: " << random << std::endl;
    srand (random);
    for(int i = 0; i < 1; i++) {
        glm::vec3* lightColor = new glm::vec3(rand() % 100,rand() % 100,rand() % 100);
        Light* light = new Light (Light::lightType::POINT, "light1", glm::vec3(rand() % 100 - 50,5,rand() % 100 - 50), *lightColor, 50, glm::vec3(0,0,1));
        light->setMesh (cubeMeshF);
        AbstractMaterial* colorMaterial2 = new ColorMaterial ( glm::normalize(*lightColor));
        //light->setBehaviour(new LookAt(teapot));
        light->setMaterial(colorMaterial2);
        _world->add(light);
    }

    Light* light3 = new Light (Light::lightType::DIRECTIONAL, "light3", glm::vec3(0,0,0), glm::vec3(0.1,0.1,0.1), 0.8f, glm::vec3(), 70);
    //light3->setMesh (cubeMeshF);
	//light3->setMaterial(new ColorMaterial(glm::normalize(glm::vec3(1, 1, 1))));
    _world->add(light3);
}

void TestScene::_render() {
    AbstractGame::_render();
	_world->debugDraw();
    _updateHud();
}

void TestScene::_updateHud() {
    string debugInfo = "";
    debugInfo += string ("FPS:") + std::to_string((int)_fps)+"\n";

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
