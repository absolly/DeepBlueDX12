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

#include "mge/materials/LitWaveMaterial.hpp"
#include "mge/materials/GPUinstancingMaterial.hpp"

#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/CameraOrbitBehaviour.hpp"
#include "mge/util/DebugHud.hpp"
#include "Content/GameObjects/FishTank.hpp"
#include "Content/Behaviours/PredatorBehaviour.h"

#include "mge/config.hpp"
#include "Content/DeferredTestScene.hpp"
#include <time.h>       /* time */
#include "mge\Config.hpp"
#include "Content\GameObjects\PlayerFishFlock.hpp"
#include "Content\GameObjects\Player.h"
#include "Content/Behaviours/Player/PlayerFishAbilityBehaviour.h"
#include "Content\GameObjects\EnvironmentSoundPlayer.h"
#include "mge\materials\UnlitTextureMaterial.hpp"
#include "mge\behaviours\RotatingBehaviour.hpp"
//construct the game class into _window, _renderer and hud (other parts are initialized by build)
DeferredTestScene::DeferredTestScene():AbstractGame (),_hud(0) {
}

void DeferredTestScene::initialize(HINSTANCE pHinstance, HINSTANCE pPrevInstance, int pShowCmd) {
    //setup the core part
    AbstractGame::initialize(pHinstance,  pPrevInstance, pShowCmd);

    //setup the custom part
    cout << "Initializing HUD" << endl;
    _hud = new DebugHud(_window);
    cout << "HUD initialized." << endl << endl;
}

//build the game _world
void DeferredTestScene::_initializeScene() {
    _renderer->setClearColor(0, 51, 102);

    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,0,0));
    //camera->rotate(glm::radians(180.0f), glm::vec3(0,1,0));
    _world->add(camera);
    _world->setMainCamera(camera);
	GameObject* go = new GameObject();
	_world->add(go);
	camera->addBehaviour(new CameraOrbitBehaviour(250, 0, 0.5, 1, go));

    //MESHES
	string fishModel = "MantaRay.obj";
	Config::updateValue("fishModel", fishModel);
	string fishTexture = "bricks.jpg";
	Config::updateValue("fishTexture", fishTexture);
	string fishMovementMap = "RayAnimUV.png";
	Config::updateValue("fishMovementMap", fishMovementMap);
	string fishNormal = "BricksNormal.png";
	Config::updateValue("fishNormal", fishNormal);
	string fishSpecular = "black.png";
	Config::updateValue("fishSpecular", fishSpecular);

	string groundModel = "plane_8192.obj";
	Config::updateValue("groundModel", groundModel);
	string groundTexture = "beachsand.jpg";
	Config::updateValue("groundTexture", groundTexture);
	string groundNormal = "BricksNormal.png";
	Config::updateValue("groundNormal", groundNormal);
	string groundSpecular = "black.png";
	Config::updateValue("groundSpecular", groundSpecular);
	float groundTiling = 1;
	Config::updateValue("groundTiling", groundTiling);

	string playerModel = "cube_flat.obj";
	Config::updateValue("playerModel", playerModel);
	string playerTexture = "bricks.jpg";
	Config::updateValue("playerTexture", playerTexture);
	string playerNormal = "BricksNormal.png";
	Config::updateValue("playerNormal", playerNormal);
	string playerSpecular = "black.png";
	Config::updateValue("playerSpecular", playerSpecular);

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (Config::MGE_MODEL_PATH+ groundModel);
    Mesh* mantaMeshF = Mesh::load (Config::MGE_MODEL_PATH+ fishModel);
	Mesh* lightMeshS = Mesh::load(Config::MGE_MODEL_PATH + playerModel);
    // Mesh* carMesh = Mesh::load(config::MGE_MODEL_PATH+"car.obj");
    //MATERIALS


    //10 specular teapot material
    //AbstractMaterial* waveMaterial = new LitWaveMaterial(Texture::load (Config::MGE_TEXTURE_PATH+ fishTexture), Texture::load(Config::MGE_TEXTURE_PATH + fishMovementMap), 1, 1, Texture::load(Config::MGE_TEXTURE_PATH + fishSpecular), Texture::load(Config::MGE_TEXTURE_PATH + fishNormal));
	//AbstractMaterial* coralMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "TubeCoral_Base.png"), 1, 0, Texture::load(config::MGE_TEXTURE_PATH + "TubeCoral_Normal.png"), Texture::load(config::MGE_TEXTURE_PATH + "TubeCoral_Normal.png"));
	//AbstractMaterial* textureMaterial = new ColorMaterial(glm::vec3(1, 1, 1));
	//AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + groundTexture), groundTiling, 1, Texture::load(Config::MGE_TEXTURE_PATH + groundSpecular), Texture::load(Config::MGE_TEXTURE_PATH + groundNormal));
	//AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + playerTexture), groundTiling, 1, Texture::load(Config::MGE_TEXTURE_PATH + playerSpecular), Texture::load(Config::MGE_TEXTURE_PATH + playerNormal));
	AbstractMaterial* unlitTextureMaterial = new UnlitTextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "grass_texture.jpg"), 4);
	AbstractMaterial* unlitColorMaterial = new ColorMaterial(glm::vec3(221,0,210));
	
	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "grass_texture.jpg"), 4, 5, Texture::load(Config::MGE_TEXTURE_PATH + "missing.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "Rocks_normal.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "yellowemission.png"));
	AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "grass_texture.jpg"), 4, 5, Texture::load(Config::MGE_TEXTURE_PATH + "missing.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "Rocks_normal.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "yellowemission.png"));
	
	//SCENE SETUP
	//GameObject* go2 = new GameObject("go");
	//go2->setMesh(planeMeshDefault);
	//go2->setMaterial(unlitTextureMaterial);
	//go2->scale(glm::vec3(0.01f));
	//_world->add(go2);
	//seed random so results are the same every time
	srand(9);
	const glm::ivec3 NUM_OBJECTS = glm::vec3(20,20,20);
	const glm::ivec3 GRID_SIZE = glm::vec3(400, 400, 400);
	for (int i = 0; i < NUM_OBJECTS.x * NUM_OBJECTS.y * NUM_OBJECTS.z; i++) {
		GameObject* go = new GameObject("go" + i, glm::vec3(
			(i % NUM_OBJECTS.x) * (GRID_SIZE.x / NUM_OBJECTS.x) - (GRID_SIZE.x/2),
			(i/(NUM_OBJECTS.x * NUM_OBJECTS.z) % NUM_OBJECTS.y) * (GRID_SIZE.y / NUM_OBJECTS.y) - (GRID_SIZE.y / 2),
			(i / NUM_OBJECTS.x % NUM_OBJECTS.z) * (GRID_SIZE.z / NUM_OBJECTS.z) - (GRID_SIZE.z / 2)
		));
		switch (i%4)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		}
		go->setMesh(lightMeshS);
		go->setMaterial(textureMaterial2);
		_world->add(go);
	}

   


	//Player* player = new Player();
	////AbstractBehaviour * playerap = new PlayerFishAbilityBehaviour(_world, player->getChildAt(0));
	////player->getChildAt(0)->addBehaviour(playerap);
	//_world->add(player);
	//_world->setMainCamera(player->getCamera());
	//RigidBody* playerRigidbody = player->getChildAt(0)->getBehaviour<RigidBody>();

	//Mesh* smallFish = Mesh::load(Config::MGE_MODEL_PATH + "fishLP.obj");

	//FishTank* fishTank = new FishTank(glm::vec3(0, 1, 0), _world, player, "", 50, 150);
	//fishTank->setMesh(smallFish);
	//AbstractMaterial * gpuinstancing = new ColorMaterial(glm::vec3(.7f,0,0.5f));
	//fishTank->setMaterial(gpuinstancing);
	//_world->add(fishTank);

	//GameObject* teapot = new GameObject("teapot", glm::vec3(11, 1, 11));
	//std::vector<glm::vec3> _waypoints;


	//float random = 9;
	//std:: cout << "random seed: " << random << std::endl;
	//srand (random);
	//for(int i = 0; i < 4; i++) {
	//    glm::vec3* lightColor = new glm::vec3(rand() % 100,rand() % 100,rand() % 100);
	//	glm::vec3 pos = glm::vec3(rand() % 100 - 50, 5, rand() % 100 - 50);
	//    Light* light = new Light (Light::lightType::POINT, "light1", pos, *lightColor, 20, glm::vec3(0,0,1));
	//	_waypoints.push_back(pos);
	//    light->setMesh (cubeMeshF);
	//    AbstractMaterial* colorMaterial2 = new ColorMaterial ((*lightColor));
	//    //light->setBehaviour(new LookAt(teapot));
	//    light->setMaterial(colorMaterial2);
	//    _world->add(light);
	//}

	//teapot->addBehaviour(new PredatorBehaviour(player,_waypoints, _world));
	/*teapot->setMesh(teapotMeshS);
	teapot->setMaterial(waveMaterial);
	_world->add(teapot);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			GameObject* teapot = new GameObject("teapot", glm::vec3(i*2, 1, j*2));
			teapot->setMesh(teapotMeshS);
			teapot->setMaterial(textureMaterial2);
			teapot->addBehaviour(new KeysBehaviour());
			_world->add(teapot);
		}
	}*/
	//FishTank* tank = new FishTank(glm::vec3(0,0,0),_world,"",20,30);



	glm::vec3* lightColor = new glm::vec3(127,239,217);
	Light* light = new Light(Light::lightType::DIRECTIONAL, "light1", glm::vec3(0,150,0), *lightColor, 3000, glm::vec3(0, 0, 1));
	light->setMesh(mantaMeshF);
	AbstractMaterial* colorMaterial2 = new ColorMaterial(*lightColor);
	light->rotate(glm::radians(-90.f), glm::vec3(1, 0, 0));
	light->addBehaviour(new RotatingBehaviour());
	//light->addBehaviour(new LookAt(glm::vec3(0.0001f,0.00001f,0.0001f)));
	light->setMaterial(colorMaterial2);
	_world->add(light);

	glm::vec3* lightColor2 = new glm::vec3( 239, 127, 217);
	Light* light2 = new Light(Light::lightType::SPOT, "light2", glm::vec3(150, 150, 0), *lightColor2, 2000, glm::vec3(0, 0, 1));
	light2->setMesh(mantaMeshF);
	AbstractMaterial* colorMaterial3 = new ColorMaterial(*lightColor2);
	light2->rotate(glm::radians(-90.f), glm::vec3(0, 1, 0));
	light2->addBehaviour(new RotatingBehaviour());
	light2->addBehaviour(new RotatingBehaviour());
	//light->addBehaviour(new LookAt(glm::vec3(0.0001f,0.00001f,0.0001f)));
	light2->setMaterial(colorMaterial3);
	_world->add(light2);

////    Light* light3 = new Light (Light::lightType::POINT, "light3", glm::vec3(10,2,-10), *lightColor, 100.f, Light::lightDirection::CONSTANT);
////    light3->setMesh (cubeMeshF);
////    light3->setMaterial(colorMaterial2);
////    _world->add(light3);

	//SoundManager * soundmng = new SoundManager();
	//EnvironmentSoundPlayer* environmentSoundPlayer = new EnvironmentSoundPlayer(*soundmng);
	//_world->add(environmentSoundPlayer);
}

void DeferredTestScene::_render() {
    AbstractGame::_render();
#ifdef API_OPENGL
	if (Config::POST_FX)
		AbstractGame::_renderToQuad();

	_updateHud();
#endif
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
 