#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/core/Light.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/CameraOrbitBehaviour.hpp"
#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "Content/DeferredTestScene.hpp"
#include <time.h>       /* time */

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
DeferredTestScene::DeferredTestScene():AbstractGame (),_hud(0) {
}

void DeferredTestScene::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part
    cout << "Initializing HUD" << endl;
    _hud = new DebugHud(_window);
    cout << "HUD initialized." << endl << endl;
}

//build the game _world
void DeferredTestScene::_initializeScene() {
    _renderer->setClearColor(0,0,0);

    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,0,0));
    camera->rotate(glm::radians(180.0f), glm::vec3(0,1,0));
    _world->add(camera);
    _world->setMainCamera(camera);

    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
	Mesh* teapotMeshS = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");
    // Mesh* carMesh = Mesh::load(config::MGE_MODEL_PATH+"car.obj");
    //MATERIALS


    //10 specular teapot material
    AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load (config::MGE_TEXTURE_PATH+"bricks.jpg"), 1, 10);

    //SCENE SETUP
 //   GameObject* plane = new GameObject("plane", glm::vec3(0,0,0));
 //   plane->scale(glm::vec3(50,50,50));
 //   plane->setMesh(planeMeshDefault);
 //   plane->setMaterial(textureMaterial2);
	//plane->addBehaviour(new KeysBehaviour());
 //   _world->add(plane);

	GameObject* teapot = new GameObject("teapot", glm::vec3(4, 1, 4));
	teapot->addBehaviour(new KeysBehaviour());
	_world->add(teapot);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			GameObject* teapot = new GameObject("teapot", glm::vec3(i*2, 1, j*2));
			teapot->setMesh(teapotMeshS);
			teapot->setMaterial(textureMaterial2);
			teapot->addBehaviour(new KeysBehaviour());
			_world->add(teapot);
		}
	}



    camera->addBehaviour(new CameraOrbitBehaviour (10, 30, 150, 1, teapot));

    float random = 9;
    std:: cout << "random seed: " << random << std::endl;
    srand (random);
    for(int i = 0; i < 23; i++) {
        glm::vec3* lightColor = new glm::vec3(rand() % 100,rand() % 100,rand() % 100);
        Light* light = new Light (Light::lightType::POINT, "light1", glm::vec3(rand() % 100 - 50,5,rand() % 100 - 50), *lightColor, 50, glm::vec3(0,0,1));
        light->setMesh (cubeMeshF);
        AbstractMaterial* colorMaterial2 = new ColorMaterial ( glm::normalize(*lightColor));
        //light->setBehaviour(new LookAt(teapot));
        light->setMaterial(colorMaterial2);
        _world->add(light);
    }
	glm::vec3* lightColor = new glm::vec3(1,1,1);
	Light* light = new Light(Light::lightType::DIRECTIONAL, "light1", glm::vec3(0,100,0), *lightColor, 2, glm::vec3(0, 0, 1));
	light->setMesh(cubeMeshF);
	AbstractMaterial* colorMaterial2 = new ColorMaterial(glm::normalize(*lightColor));
	light->addBehaviour(new LookAt(glm::vec3(0,0,0)));
	light->setMaterial(colorMaterial2);
	_world->add(light);

//    Light* light3 = new Light (Light::lightType::POINT, "light3", glm::vec3(10,2,-10), *lightColor, 100.f, Light::lightFalloff::CONSTANT);
//    light3->setMesh (cubeMeshF);
//    light3->setMaterial(colorMaterial2);
//    _world->add(light3);
}

void DeferredTestScene::_render() {
    AbstractGame::_render();
	_updateHud();
	AbstractGame::_renderToQuad();
}

void DeferredTestScene::_updateHud() {
    string debugInfo = "";
    debugInfo += string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

DeferredTestScene::~DeferredTestScene() {
    //dtor
}
