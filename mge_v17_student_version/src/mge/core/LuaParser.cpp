#include "LuaParser.hpp"
#include "mge/config.hpp"
#include "mge/core/Light.hpp"
#include "mge\core\Physics\CollisionBehaviour.h"
#include "mge\core\Physics\PhysicsWorld.h"
#include "mge\materials\GPUinstancingMaterial.hpp"
#include "Content/GameObjects/FishTank.hpp"
#include "World.hpp"
#include "Content/Behaviours/PredatorBehaviour.h"
#include "mge/materials/LitWaveMaterial.hpp"
#include "Content\GameObjects\Player.h"
#include "mge\core\Physics\RigidBody.hpp"
#include "mge\core\Physics\Colliders\Collider.h"
#include <algorithm>

std::map<std::string, std::vector<GameObject*>> LuaParser::groups;

//------------------------------------------------------------------------------------------------------------
//                                                      LuaParser()
//------------------------------------------------------------------------------------------------------------
LuaParser::LuaParser(World* pWorld) {
	_world = pWorld;
	lua = luaL_newstate();
	luaL_openlibs(lua);

	smallFish = Mesh::load(Config::MGE_MODEL_PATH + "fishLP.obj");
	//gpuinstancing = new GPUinstancingMaterial(*fishTank->allFish);


	*static_cast<LuaParser**>(lua_getextraspace(lua)) = &(*this); //store 'this' in lua extra space

	lua_pushcfunction(lua, &dispatch<&LuaParser::createObject>); //binding to c++ member via 'dispatch' wrapper
	lua_setglobal(lua, "createObject"); //function name in lua
	lua_pushcfunction(lua, &dispatch<&LuaParser::setMainCharacter>);
	lua_setglobal(lua, "setmaincharacter");
	lua_pushcfunction(lua, &dispatch<&LuaParser::addlistener>);
	lua_setglobal(lua, "addlistener");
	lua_pushcfunction(lua, &dispatch<&LuaParser::destoryLuaObject>);
	lua_setglobal(lua, "destroy");
	lua_pushcfunction(lua, &dispatch<&LuaParser::addMaterial>);
	lua_setglobal(lua, "addMaterial");
	lua_pushcfunction(lua, &dispatch<&LuaParser::addMeshCollider>);
	lua_setglobal(lua, "addMeshCollider");
	lua_pushcfunction(lua, &dispatch<&LuaParser::addBoxCollider>);
	lua_setglobal(lua, "addBoxCollider");
	lua_pushcfunction(lua, &dispatch<&LuaParser::addSphereCollider>);
	lua_setglobal(lua, "addSphereCollider");
	lua_pushcfunction(lua, &dispatch<&LuaParser::createTrigger>);
	lua_setglobal(lua, "createTrigger");
	lua_pushcfunction(lua, &dispatch<&LuaParser::createFish>);
	lua_setglobal(lua, "createFish");
	lua_pushcfunction(lua, &dispatch<&LuaParser::createLight>);
	lua_setglobal(lua, "createLight");
	lua_pushcfunction(lua, &dispatch<&LuaParser::addLightAttributes>);
	lua_setglobal(lua, "addLightAttributes");
	lua_pushcfunction(lua, &dispatch<&LuaParser::addPredator>);
	lua_setglobal(lua, "addPredator");
	lua_pushcfunction(lua, &dispatch<&LuaParser::addToGroup>);
	lua_setglobal(lua, "addToGroup");
	//load the cube mesh and texture for later use.
	cubeMeshF = Mesh::load(Config::MGE_MODEL_PATH + "cube_unity.obj");
	textureMaterial2 = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"));

	//bind colors to names
	colors["brown"] = glm::vec3(81, 21, 21);
	colors["gray"] = glm::vec3(42, 42, 42);
	colors["green"] = glm::vec3(42, 162, 42);
	colors["ltgreen"] = glm::vec3(50, 200, 50);
	colors["red"] = glm::vec3(162, 0, 0);
	colors["yellow"] = glm::vec3(162, 162, 42);
}

//------------------------------------------------------------------------------------------------------------
//                                                      LoadFile()
//                                    loads a lua file and run its start function
//------------------------------------------------------------------------------------------------------------
void LuaParser::loadFile(const char* pFileName) {
	//load the file
	if (luaL_loadfile(lua, pFileName)) {
		std::cout << "Unable to find lua file: " << pFileName << std::endl;
		throw std::runtime_error("Unable to find lua file");
	}

	//protected call, report back any errors
	int error = lua_pcall(lua, 0, 0, 0);
	if (error != 0) {
		std::cout << "stack = " << lua_gettop(lua) << "\n";
		std::cout << "error = " << error << "\n";
		std::cout << "message = " << lua_tostring(lua, -1) << "\n";
		lua_pop(lua, 1);
	}

	//get the start function and execute it
	////lua_getglobal(lua, "start");
	//if(lua_isnil(lua,-1)) {
	//    std::cout << "function 'start' does not exist" << std::endl;
	//    lua_settop(lua, 0);
	//    return;
	//}

	////again check for errors
	//error = lua_pcall(lua, 0, 0, 0);
	//if(error != 0) {
	//    std::cout << "stack = " << lua_gettop(lua) << "\n";
	//    std::cout << "error = " << error << "\n";
	//    std::cout << "message = " << lua_tostring(lua, -1) << "\n";
	//    lua_pop(lua, 1);
	//}

	//*static_cast<LuaParser**>(lua_getextraspace(lua)) = &(*this); //store 'this' in lua extra space

	//lua_pushcfunction(lua, &dispatch<&LuaParser::write>); //binding to c++ member via 'dispatch' wrapper
	//lua_setglobal(lua, "image"); //function name in lua

	//luaL_dostring(lua, "image('1,2,3')");

	//luaL_loadfile(lua, pFileName);

	//luaL_dostring(lua, "createObject('Stone', 105,7.5,-65, 15,15,15, 'gray' )");

	//lua_call(lua, 0, 0);
	//lua_getglobal(lua, "main");
	//lua_call(lua, 0, 0);
}

//------------------------------------------------------------------------------------------------------------
//                                                      update()
//                                  Checks for collisions in the list and fires the lua events
//------------------------------------------------------------------------------------------------------------
void LuaParser::update(float pStep) {
	//for-each listener in the collisionListeners
	for (collisionListener listener : collisionListeners) {
		//check if both are still valid(have a parent) and then check if they collide
		if (listener.objectA->getParent() != NULL && listener.objectB->getParent() != NULL && hitTest(listener.objectA, listener.objectB)) {
			//output the event listener name
			std::cout << listener.eventListener << std::endl;

			//call the event listener function in lua
			lua_getglobal(lua, listener.eventListener);
			if (lua_isnil(lua, -1)) {
				std::cout << "function " << listener.eventListener << " does not exist" << std::endl;
				lua_settop(lua, 0);
				return;
			}

			//push the two parameters
			lua_pushlightuserdata(lua, listener.objectA);
			lua_pushlightuserdata(lua, listener.objectB);

			//check for errors with protected call
			int error = lua_pcall(lua, 2, 0, 0);
			if (error != 0) {
				std::cout << "stack = " << lua_gettop(lua) << "\n";
				std::cout << "error = " << error << "\n";
				std::cout << "message = " << lua_tostring(lua, -1) << "\n";
				lua_pop(lua, 1);
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------
//                                                      hitTest()
//                                    Checks for collision between two GameObjects
//                                      Returns true if the two objects collide
//------------------------------------------------------------------------------------------------------------
bool LuaParser::hitTest(GameObject* objectA, GameObject* objectB) {
	float distance = glm::distance(objectA->getLocalPosition(), objectB->getLocalPosition());
	if (distance > 1) return false;
	return true;
}

//------------------------------------------------------------------------------------------------------------
//                                                      setMainCharacter()
//                   Enables keyboard controls(arrow keys for movement and shift and ctrl for up and down)
//                                Also creates a camera that orbits the main character
//------------------------------------------------------------------------------------------------------------
int LuaParser::setMainCharacter(lua_State * lua) {
	//check the parameter
	if (!lua_islightuserdata(lua, -1)) {
		lua_pushstring(lua, "invalid parameter for fucntion 'setMainCharacter', should be of type 'lightuserdata'");
		lua_error(lua);
	}

	//get the gameobject and set the behaviour
	GameObject * mainCharacter = (GameObject*)lua_touserdata(lua, -1);
	mainCharacter->addBehaviour(new KeysBehaviour());

	//create a camera and set its behaviour to oribit the main character
	Camera* camera = new Camera("camera", glm::vec3(0, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);
	camera->addBehaviour(new CameraOrbitBehaviour(10, 30, 150, 1, mainCharacter));

	//returns nothing to lua
	return 0;
}

//------------------------------------------------------------------------------------------------------------
//                                                      addlistener()
//                                        adds an collision listener to the list
//                                             Does not return anything to lua
//------------------------------------------------------------------------------------------------------------
int LuaParser::addlistener(lua_State * lua) {
	//check if the right amount of parameters where given
	if (lua_gettop(lua) != 3) {
		lua_pushstring(lua, "invalid parameter count for function 'addlistener'");
		lua_error(lua);
		return 0;
	}

	//check if parameters are of correct type
	if (!lua_islightuserdata(lua, -3)) {
		lua_pushstring(lua, "invalid parameter 1");
		lua_error(lua);
		return 0;
	}
	if (!lua_islightuserdata(lua, -2)) {
		lua_pushstring(lua, "invalid parameter 2");
		lua_error(lua);
		return 0;
	}
	if (!lua_isstring(lua, -1)) {
		lua_pushstring(lua, "invalid parameter 3");
		lua_error(lua);
		return 0;
	}

	//add the listener to the vector
	collisionListeners.push_back(collisionListener((GameObject*)lua_touserdata(lua, -3), (GameObject*)lua_touserdata(lua, -2), lua_tostring(lua, -1)));
	return 0;
}

//------------------------------------------------------------------------------------------------------------
//                                                      createObject()
//                                Create an object from lua, returns lightuserdata to lua
//                                   This function returns the created gameobject to lua
//------------------------------------------------------------------------------------------------------------
int LuaParser::createObject(lua_State * lua) {

	//int params = lua_gettop(lua);
	//float x;
	//float y;
	//float z;
	//float scaleX = 1;
	//float scaleY = 1;
	//float scaleZ = 1;
	//std::string name;
	//std::string colorName;

	////if 8 parameters are given we expect the scale to be included
	//if(params == 8) {
	//    if(!lua_isstring(lua, -8)) {
	//        lua_pushstring(lua, "invalid parameter 1");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isstring(lua, -7)) {
	//        lua_pushstring(lua, "invalid parameter 2");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isstring(lua, -6)) {
	//        lua_pushstring(lua, "invalid parameter 3");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isstring(lua, -5)) {
	//        lua_pushstring(lua, "invalid parameter 4");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isstring(lua, -4)) {
	//        lua_pushstring(lua, "invalid parameter 5");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isstring(lua, -3)) {
	//        lua_pushstring(lua, "invalid parameter 3");
	//        lua_error(lua);
	//        return 0;
	//        lua_pushstring(lua, "invalid parameter 6");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isstring(lua, -2)) {
	//        lua_pushstring(lua, "invalid parameter 7");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isstring(lua, -1)) {
	//        lua_pushstring(lua, "invalid parameter 8");
	//        lua_error(lua);
	//        return 0;
	//    }

	//    name = lua_tostring(lua,-8);
	//    x = lua_tonumber(lua, -7);
	//    y = lua_tonumber(lua, -6);
	//    z = lua_tonumber(lua, -5);
	//    scaleX = lua_tonumber(lua, -4);
	//    scaleY = lua_tonumber(lua, -3);
	//    scaleZ = lua_tonumber(lua, -2);
	//    colorName = lua_tostring(lua, -1);

	//}
	////if 5 parameters where given then we expect the scale to be (1,1,1)
	//else if(params == 5) {
	//    if(!lua_isstring(lua, -5)) {
	//        lua_pushstring(lua, "invalid parameter 1");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isnumber(lua, -4)) {
	//        lua_pushstring(lua, "invalid parameter 2");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isnumber(lua, -3)) {
	//        lua_pushstring(lua, "invalid parameter 3");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isnumber(lua, -2)) {
	//        lua_pushstring(lua, "invalid parameter 4");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    if(!lua_isstring(lua, -1)) {
	//        lua_pushstring(lua, "invalid parameter 5");
	//        lua_error(lua);
	//        return 0;
	//    }
	//    name = lua_tostring(lua,-5);
	//    x = lua_tonumber(lua, -4);
	//    y = lua_tonumber(lua, -3);
	//    z = lua_tonumber(lua, -2);
	//    colorName = lua_tostring(lua, -1);
	//} else {
	//    lua_pushstring(lua, "invalid parameter count for function 'createObject'");
	//    lua_error(lua);
	//    return 0;
	//}

	////create color material with color defined in the table, the color materials are created here since we might want to transition to color codes later.
	ColorMaterial* colorMat = new ColorMaterial(glm::vec3(1, 0, 0));

	////print the objects debug info to the console
	//std::cout << "name: " << name << "\tpos: " << x << "," << y << "," << -z << "\t" << "scale: " << scaleX << "," << scaleY << "," << scaleZ << "\t" << "color: " << colorName << std::endl;

	////create the gameobject, the z is negative to represent unity's axis system
	//GameObject* go = new GameObject(name, glm::vec3(x,y,-z));
	//go->scale(glm::vec3(scaleX,scaleY,scaleZ));

	//_world->add(go);

	////return the object to lua
	//lua_pushlightuserdata(lua, go);

	float m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;

	m11 = lua_tonumber(lua, -16);
	m12 = lua_tonumber(lua, -15);
	m13 = lua_tonumber(lua, -14);
	m14 = lua_tonumber(lua, -13);
	m21 = lua_tonumber(lua, -12);
	m22 = lua_tonumber(lua, -11);
	m23 = lua_tonumber(lua, -10);
	m24 = lua_tonumber(lua, -9);
	m31 = lua_tonumber(lua, -8);
	m32 = lua_tonumber(lua, -7);
	m33 = lua_tonumber(lua, -6);
	m34 = lua_tonumber(lua, -5);
	m41 = lua_tonumber(lua, -4);
	m42 = lua_tonumber(lua, -3);
	m43 = lua_tonumber(lua, -2);
	m44 = lua_tonumber(lua, -1);
	string meshName = lua_tostring(lua, -17);
	string name = lua_tostring(lua, -18);

	GameObject* go = new GameObject(name, glm::vec3(0, 0, 0));
	go->setTransform(glm::transpose(glm::mat4(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, -m34, m41, m42, m43, m44)));
	go->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));

	if (meshName != "null")
	{
		Mesh * gameObjectMesh = Mesh::load(Config::MGE_MODEL_PATH + meshName + ".obj");
		go->setMesh(gameObjectMesh);
		go->setMaterial(colorMat);
	}

	_currentGameObject = go;
	AbstractMaterial* purpleColor = new ColorMaterial(glm::vec3(1, 0, 1));
	_currentGameObject->setMaterial(purpleColor);
	_world->add(go);
	std::cout << go->getName() << std::endl;

	std::cout << "Create Object: " << meshName << std::endl;
	//if (meshName == "ShipSide1")
	//{
	//	go->rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));
	//}


	return 1;
}

void LuaParser::setPlayerRigidBody(Player &player)
{
	_player = &player;
	_playerRigidBody = player.getChildAt(0)->getBehaviour<RigidBody>();
}

int LuaParser::addMaterial(lua_State * lua) {
	string image = lua_tostring(lua, -4);
	string normalMap = lua_tostring(lua, -3);
	string SpecularMap = lua_tostring(lua, -2);
	float SpecularMultiplier = lua_tonumber(lua, -1);

	AbstractMaterial* textureMaterial;

	if (normalMap == "null" && SpecularMap == "null")
	{
		textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + image + ".jpg"), 1, SpecularMultiplier);
	}
	else
	{
		if (normalMap == "null" && SpecularMap != "null")
		{
			textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + image + ".jpg"), 1, SpecularMultiplier, Texture::load(Config::MGE_TEXTURE_PATH + SpecularMap + ".png"));
		}
		else if (normalMap != "null" && SpecularMap == "null")
		{
			textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + image + ".jpg"), 5, SpecularMultiplier, Texture::load(Config::MGE_TEXTURE_PATH + "white" + ".png"), Texture::load(Config::MGE_TEXTURE_PATH + normalMap + ".jpg"));
		}
		else
		{
			textureMaterial = new TextureMaterial(Texture::load(Config::MGE_TEXTURE_PATH + image + ".jpg"), 1, SpecularMultiplier, Texture::load(Config::MGE_TEXTURE_PATH + SpecularMap + ".png"), Texture::load(Config::MGE_TEXTURE_PATH + normalMap + ".jpg"));
		}
	}

	_currentGameObject->setMaterial(textureMaterial);

	return 1;
}
int LuaParser::addMeshCollider(lua_State * lua) {
	string collider = lua_tostring(lua, -2);
	bool isTrigger = lua_toboolean(lua, -1);

	Collider& objectCollider = _currentGameObject->addCollider(MeshColliderArgs(*_currentGameObject->getMesh()), isTrigger, true);

	if (isTrigger)
		objectCollider.collisionEvents[_playerRigidBody].bind(scriptParser, &LuaScriptParser::printTest);

	return 1;
}

int LuaParser::addBoxCollider(lua_State * lua) {
	bool isTrigger = lua_toboolean(lua, -1);
	float x = lua_tonumber(lua, -7);
	float y = lua_tonumber(lua, -6);
	float z = lua_tonumber(lua, -5);

	Collider& objectCollider = _currentGameObject->addCollider(BoxColliderArgs(x / 2, y / 2, z / 2), isTrigger, true);

	if (isTrigger)
		objectCollider.collisionEvents[_playerRigidBody].bind(scriptParser, &LuaScriptParser::printTest);

	return 1;
}

int LuaParser::addToGroup(lua_State * lua)
{
	std::string groupName = lua_tostring(lua, -1);
	if (groupName != "")
	{
		if (groups.find(groupName) == groups.end())
			groups[groupName] = std::vector<GameObject*>();
		groups[groupName].push_back(_currentGameObject);

		std::cout << std::endl << "Object added to group: " << groupName << std::endl;
	}
	return 1;
}

int LuaParser::addSphereCollider(lua_State * lua) {
	float radius = lua_tonumber(lua, -5);
	bool isTrigger = lua_toboolean(lua, -1);

	Collider& objectCollider = _currentGameObject->addCollider(SphereColliderArgs(radius), isTrigger, true);

	if (isTrigger)
		objectCollider.collisionEvents[_playerRigidBody].bind(scriptParser, &LuaScriptParser::printTest);

	return 1;
}


int LuaParser::createFish(lua_State * lua)
{
	float x = lua_tonumber(lua, -3);
	float y = lua_tonumber(lua, -2);
	float z = lua_tonumber(lua, -1);

	Mesh* smallFish = Mesh::load(Config::MGE_MODEL_PATH + "fishLP.obj");

	FishTank* fishTank = new FishTank(glm::vec3(x, y, z), _world, "", 100, 150);
	fishTank->setMesh(smallFish);
	fishTank->setMaterial(gpuinstancing);
	_world->add(fishTank);

	return 1;
}


int LuaParser::createTrigger(lua_State * lua)
{
	std::cout << "Create Trigger" << std::endl;

	return 1;
}


int LuaParser::createLight(lua_State * lua)
{
	float m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;

	m11 = lua_tonumber(lua, -16);
	m12 = lua_tonumber(lua, -15);
	m13 = lua_tonumber(lua, -14);
	m14 = lua_tonumber(lua, -13);
	m21 = lua_tonumber(lua, -12);
	m22 = lua_tonumber(lua, -11);
	m23 = lua_tonumber(lua, -10);
	m24 = lua_tonumber(lua, -9);
	m31 = lua_tonumber(lua, -8);
	m32 = lua_tonumber(lua, -7);
	m33 = lua_tonumber(lua, -6);
	m34 = lua_tonumber(lua, -5);
	m41 = lua_tonumber(lua, -4);
	m42 = lua_tonumber(lua, -3);
	m43 = lua_tonumber(lua, -2);
	m44 = lua_tonumber(lua, -1);
	string LightType = lua_tostring(lua, -17);

	Light::lightType lighttype;

	if (LightType == "Directional")
	{
		lighttype = Light::lightType::DIRECTIONAL;
	}

	if (LightType == "Point")
	{
		lighttype = Light::lightType::POINT;
	}

	if (LightType == "Spot")
	{
		lighttype = Light::lightType::SPOT;
	}

	Light* light = new Light(lighttype, "light1", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 50, glm::vec3(0, 0, 1));
	light->setMesh(cubeMeshF);
	light->setTransform(glm::transpose(glm::mat4(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, -m34, m41, m42, m43, m44)));
	_currentLight = light;
	_world->add(light);

	return 1;
}

int LuaParser::addLightAttributes(lua_State * lua)
{
	float LightRange = lua_tonumber(lua, -6);
	float LightIntensity = lua_tonumber(lua, -5);
	float r = lua_tonumber(lua, -4);
	float b = lua_tonumber(lua, -3);
	float g = lua_tonumber(lua, -2);
	float a = lua_tonumber(lua, -1);

	_currentLight->setLightIntensity(LightIntensity);
	_currentLight->setColor(glm::vec3(r, g, b));

	return 1;
}

//------------------------------------------------------------------------------------------------------------
//                                                      destroyLuaObject()
//                             Check if object wasn't destroyed already and then remove/delete it
//                                          This function does not return anything to lua
//------------------------------------------------------------------------------------------------------------
int LuaParser::destoryLuaObject(lua_State * lua) {
	if (!lua_islightuserdata(lua, -1)) {
		lua_pushstring(lua, "invalid parameter for 'destroy'");
		lua_error(lua);
		return 0;
	}

	GameObject* go = (GameObject*)lua_touserdata(lua, -1);
	if (go->getParent() != NULL) {
		_world->remove(go);
		delete(go);
	}

	return 0;
}

int LuaParser::addPredator(lua_State * lua)
{
	std::cout << "adding predator" << std::endl;
	vector<glm::vec3> waypoints;
	int count = lua_tonumber(lua, -1);
	for (int i = 0; i <= count - 1; i++) {
		glm::vec3 pos;
		for (int j = 0; j < 3; j++) {
			//std::cout << "pos: " << (-((i * 3) + j + 2)) << " | " << i << "," << j << std::endl;
			pos[2 - j] = lua_tonumber(lua, -((i * 3) + j + 2));
			if (j == 0)
				pos[2 - j] = -pos[2 - j];
		}
		waypoints.push_back(pos);
	}
	std::reverse(waypoints.begin(), waypoints.end());
	for each (glm::vec3 waypoint in waypoints)
		std::cout << "waypoint: " << waypoint << std::endl;

	Mesh* teapotMeshS = Mesh::load(Config::MGE_MODEL_PATH + "MantaRay.obj");
	AbstractMaterial* waveMaterial = new LitWaveMaterial(Texture::load(Config::MGE_TEXTURE_PATH + "bricks.jpg"), Texture::load(Config::MGE_TEXTURE_PATH + "RayAnimUV.png"), 1, 1, Texture::load(Config::MGE_TEXTURE_PATH + "black.png"), Texture::load(Config::MGE_TEXTURE_PATH + "BricksNormal.png"));

	GameObject* predator = new GameObject("", waypoints[0]);
	predator->addBehaviour(new PredatorBehaviour(_player, waypoints, _world));
	predator->setMesh(teapotMeshS);
	predator->setMaterial(waveMaterial);
	_world->add(predator);
	return 0;
}
