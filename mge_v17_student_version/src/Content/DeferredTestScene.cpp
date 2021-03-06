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
	camera->addBehaviour(new CameraOrbitBehaviour(80, 0, 0.5, 1, go));

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
    Mesh* coral1Mesh = Mesh::load(Config::MGE_MODEL_PATH + "BrainCoral.obj");
	Mesh* coral2Mesh = Mesh::load(Config::MGE_MODEL_PATH + "TubeCoral.OBJ");
    Mesh* coral3Mesh = Mesh::load(Config::MGE_MODEL_PATH + "TableCoral.obj");
    Mesh* coral4Mesh = Mesh::load(Config::MGE_MODEL_PATH + "StaghornCoral.OBJ");
	Mesh* coneMesh = Mesh::load(Config::MGE_MODEL_PATH + "dive_scooter.obj");

    //MATERIALS
	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "BrainCoral_BaseBEIG.png"), 1, 0, Texture::load(Config::MGE_TEXTURE_PATH + "black.png"), Texture::load(Config::MGE_TEXTURE_PATH + "BrainCoral_Normal2.png"), Texture::load(Config::MGE_TEXTURE_PATH + "yellowemission.png"));
	AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "TubeCoral_BasePURP.png"), 1, 5, Texture::load(Config::MGE_TEXTURE_PATH + "testtextue.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "TableCoral_Normal.png"), Texture::load(Config::MGE_TEXTURE_PATH + "yellowemission.png"));
	AbstractMaterial* textureMaterial3 = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "TableCoral_BaseBLU.png"), 1, 5, Texture::load(Config::MGE_TEXTURE_PATH + "testtextue.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "TableCoral_Normal.png"), Texture::load(Config::MGE_TEXTURE_PATH + "yellowemission.png"));
	AbstractMaterial* textureMaterial4 = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "Staghorn_BaseYEL.png"), 1, 5, Texture::load(Config::MGE_TEXTURE_PATH + "testtextue.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "TableCoral_Normal.png"), Texture::load(Config::MGE_TEXTURE_PATH + "yellowemission.png"));

	AbstractMaterial* unlitTextureMaterial = new UnlitTextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "BrainCoral_BaseBEIG.png"), 1);
	AbstractMaterial* unlitTextureMaterial2 = new UnlitTextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "TubeCoral_BasePURP.png"), 1);
	AbstractMaterial* unlitTextureMaterial3 = new UnlitTextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "TableCoral_BaseBLU.png"), 1);
	AbstractMaterial* unlitTextureMaterial4 = new UnlitTextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "Staghorn_BaseYEL.png"), 1);


	//SCENE SETUP
	//GameObject* go2 = new GameObject("go");
	//go2->setMesh(planeMeshDefault);
	//go2->setMaterial(textureMaterial);
	//go2->scale(glm::vec3(0.01f));
	//_world->add(go2);
	//seed random so results are the same every time
	srand(9);
	const glm::ivec3 NUM_OBJECTS = glm::vec3(20, 20, 20);
	const glm::ivec3 GRID_SIZE = glm::vec3(60, 60, 60);
	for (int i = 0; i < NUM_OBJECTS.x * NUM_OBJECTS.y * NUM_OBJECTS.z; i++) {
		GameObject* go = new GameObject("go" + i, glm::vec3(
			(i % NUM_OBJECTS.x) * (GRID_SIZE.x / NUM_OBJECTS.x) - (GRID_SIZE.x/2),
			((i/(NUM_OBJECTS.x * NUM_OBJECTS.z)) % NUM_OBJECTS.y) * (GRID_SIZE.y / NUM_OBJECTS.y) - (GRID_SIZE.y / 2),
			((i / NUM_OBJECTS.x) % NUM_OBJECTS.z) * (GRID_SIZE.z / NUM_OBJECTS.z) - (GRID_SIZE.z / 2)
		));
		go->rotate(rand() % 360, glm::vec3(1, 1, 0));
		go->addBehaviour(new RotatingBehaviour());
		switch (0)
		{
		case 0:
			switch (i % 4) {
			case 0:
				go->setMaterial(textureMaterial);
				break;
			case 1:
				go->setMaterial(textureMaterial2);
				break;
			case 2:
				go->setMaterial(textureMaterial3);
				break;
			case 3:
				go->setMaterial(textureMaterial4);
				break;
			}
			break;
		case 1:
			switch (i % 4) {
			case 0:
				go->setMaterial(unlitTextureMaterial);
				break;
			case 1:
				go->setMaterial(unlitTextureMaterial2);
				break;
			case 2:
				go->setMaterial(unlitTextureMaterial3);
				break;
			case 3:
				go->setMaterial(unlitTextureMaterial4);
				break;
			}
			break;
		case 2:
			go->setMaterial(textureMaterial2);
			break;
		case 3:
			AbstractMaterial* unlitColorMaterial = new ColorMaterial(glm::vec3(rand()%255, rand() % 255, rand() % 255));
			go->setMaterial(unlitColorMaterial);
			break;
		}
		switch (i % 4)
		{
		case 0:
			go->setMesh(coral1Mesh);
			break;
		case 1:
			go->setMesh(coral2Mesh);
			break;
		case 2:
			go->setMesh(coral3Mesh);
			go->scale(glm::vec3(0.7f));
			break;
		case 3:
			go->setMesh(coral4Mesh);
			go->translate(glm::vec3(.5, 0, 0));
			break;
		}
		
		_world->add(go);
	}
	///directional light
	glm::vec3* dirLightColor = new glm::vec3(127,239,217);
	Light* light = new Light(Light::lightType::DIRECTIONAL, "dirLight", glm::vec3(0,150,0), *dirLightColor, 11, glm::vec3(0, 0, 1));
	AbstractMaterial* colorMaterial2 = new ColorMaterial(*dirLightColor);
	light->rotate(glm::radians(-90.f), glm::vec3(1, 0, 0));
	//light->setMaterial(colorMaterial2);
	_world->add(light);

	///point lights(9, grid corners and center)
	const glm::ivec3 POINT_LIGHT_DIST = (glm::ivec3)((glm::vec3)GRID_SIZE * 0.52f);
	for (int i = 0; i < 9; i++) {
		glm::vec3 pointLightColor = glm::normalize(glm::vec3(rand() % 255, rand() % 255, rand() % 255));
		glm::vec3 postion;
		switch (i)
		{
		case 0:
			postion = POINT_LIGHT_DIST * glm::ivec3(1, 1, 1);
			break;
		case 1:
			postion = POINT_LIGHT_DIST * glm::ivec3(-1, 1, 1);
			break;
		case 2:
			postion = POINT_LIGHT_DIST * glm::ivec3(1, -1, 1);
			break;
		case 3:
			postion = POINT_LIGHT_DIST * glm::ivec3(1, 1, -1);
			break;
		case 4:
			postion = POINT_LIGHT_DIST * glm::ivec3(-1, -1, 1);
			break;
		case 5:
			postion = POINT_LIGHT_DIST * glm::ivec3(1, -1, -1);
			break;
		case 6:
			postion = POINT_LIGHT_DIST * glm::ivec3(-1, 1, -1);
			break;
		case 7:
			postion = POINT_LIGHT_DIST * glm::ivec3(-1, -1, -1);
			break;
		case 8:
			postion = glm::vec3(0);
			break;
		}
		Light* pointLight = new Light(Light::lightType::POINT, "pointLight" + i, postion, pointLightColor, 22, glm::vec3(0, 20, 1));
		//pointLight->setMesh(coral1Mesh);
		//pointLight->setMaterial(new ColorMaterial(pointLightColor * 255));
		_world->add(pointLight);

	}

	///spot lights(8, all grid sides, spining)
	const glm::ivec3 SPOT_LIGHT_DIST = (glm::ivec3)((glm::vec3)GRID_SIZE * 0.8f);
	for (int i = 0; i < 6; i++) {
		glm::vec3 spotLightColor = glm::normalize(glm::vec3(rand() % 255, rand() % 255, rand() % 255));
		glm::vec3 postion;
		glm::vec3 rot;
		int y = 0;
		switch (i)
		{
		case 0:
			postion = SPOT_LIGHT_DIST * glm::ivec3(1, 0, 0);
			rot = glm::ivec3(1, 0, 0);
			break;
		case 1:
			postion = SPOT_LIGHT_DIST * glm::ivec3(0, 1, 0);
			rot = glm::ivec3(0, 1, 0);
			y = 1;
			break;
		case 2:
			postion = SPOT_LIGHT_DIST * glm::ivec3(0, 0, 1);
			rot = glm::ivec3(0, 0, 1);
			break;
		case 3:
			postion = SPOT_LIGHT_DIST * glm::ivec3(-1, 0, 0);
			rot = glm::ivec3(-1, 0, 0);
			break;
		case 4:
			postion = SPOT_LIGHT_DIST * glm::ivec3(0, -1, 0);
			rot = glm::ivec3(0, -1, 0);
			y = -1;
			break;
		case 5:
			postion = SPOT_LIGHT_DIST * glm::ivec3(0, 0, -1);
			rot = glm::ivec3(0, 0, -1);
			break;
		}
		Light* spotLight = new Light(Light::lightType::SPOT, "spotLight" + i, postion, spotLightColor, 44, glm::vec3(0, 50, 1), 60);
		//spotLight->setMesh(coneMesh);
		//spotLight->setMaterial(new ColorMaterial(spotLightColor * 255));
		if (y != 0) {
			spotLight->rotate(90, glm::vec3(y, 0, 0));
		}
		else
			spotLight->addBehaviour(new LookAt(spotLight->getWorldPosition() + rot));
		_world->add(spotLight);
	}
}

void DeferredTestScene::_render() {
    AbstractGame::_render();
#ifdef API_OPENGL
	if (Config::POST_FX)
		AbstractGame::_renderToQuad();

	//_updateHud();
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
 