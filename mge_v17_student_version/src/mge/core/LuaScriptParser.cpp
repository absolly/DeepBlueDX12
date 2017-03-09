#include "LuaScriptParser.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <mge/behaviours/AbstractBehaviour.hpp>
#include <lua.hpp>
#include <time.h>
#include <random>
#include <cassert>
#include <iostream>
#include <string>
#include "mge\core\LuaParser.hpp"
#include "mge\core\Physics\PhysicsWorld.h"
#include "Content\Hud\Hud.hpp"
#include "mge\core\Random.h"
#include "Content\Core\Input.h"
#include "mge\core\Physics\Colliders\Collider.h"
#include "Content/TestScene.hpp"
//------------------------------------------------------------------------------------------------------------
//                                                      LuaParser()
//------------------------------------------------------------------------------------------------------------
LuaScriptParser::LuaScriptParser(const char* pFileName, sf::RenderWindow * aWindow, SoundManager * pSoundManager)
{
	_messageBoxManager = new MessageBoxManager(aWindow);
	_soundManager = pSoundManager;
	TestScene::resetEvent.bind(this, &LuaScriptParser::resetLevel);
	inilua(pFileName);
}

void LuaScriptParser::inilua(const char* pFileName)
{
	lua = luaL_newstate();
	luaL_openlibs(lua);

	//*static_cast<LuaParser**>(lua_getextraspace(lua)) = &(*this); //store 'this' in lua extra space

	//lua_pushcfunction(lua, &dispatch<&LuaParser::write>); //binding to c++ member via 'dispatch' wrapper
	//lua_setglobal(lua, "image"); //function name in lua

	//luaL_dostring(lua, "image('1,2,3')");

	setup(lua);

	luaL_loadfile(lua, pFileName);

	lua_call(lua, 0, 0);
	lua_getglobal(lua, "main");
	lua_call(lua, 0, 0);
}

//------------------------------------------------------------------------------------------------------------
//                                                      write()
//------------------------------------------------------------------------------------------------------------
void LuaScriptParser::setup(lua_State * lua) {
	*static_cast<LuaScriptParser**>(lua_getextraspace(lua)) = &(*this);

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::visit>);
	lua_setglobal(lua, "visit");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::message>);
	lua_setglobal(lua, "message");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::showObjectiveDistance>);
	lua_setglobal(lua, "showObjectiveDistance");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::playSound>);
	lua_setglobal(lua, "playSound");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::destroyGroup>);
	lua_setglobal(lua, "destroyGroup");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::spawnGroup>);
	lua_setglobal(lua, "spawnGroup");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::destroy>);
	lua_setglobal(lua, "destroy");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::addCoin>);
	lua_setglobal(lua, "addCoin");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::setInteractionText>);
	lua_setglobal(lua, "setInteractionText");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::setSubtitleText>);
	lua_setglobal(lua, "setSubtitleText"); 

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::addItemToInventory>);
	lua_setglobal(lua, "addItemToInventory");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::removeItemFromInventory>);
	lua_setglobal(lua, "removeItemFromInventory");

	lua_pushcfunction(lua, &dispatch<&LuaScriptParser::setFishHintText>);
	lua_setglobal(lua, "setFishHintText");
}

void LuaScriptParser::setSoundManager(SoundManager * pSoundManager)
{
	_soundManager = pSoundManager;
}

int LuaScriptParser::playSound(lua_State * lua)
{
	std::string song = lua_tostring(lua, -8);
	std::string channel = lua_tostring(lua, -7);
	bool loop = lua_toboolean(lua, -6);
	bool interrupt = lua_toboolean(lua, -5);
	int volume = lua_tonumber(lua, -4);
	float minPitch = lua_tonumber(lua, -3);
	float maxPitch = lua_tonumber(lua, -2);
	std::string subtitle = lua_tostring(lua, -1);

	bool repeatedSong = (_lastSong == song);
	std::cout << "Lua called play sound" << std::endl;
	_soundManager->PlaySound(song, channel, loop, interrupt, repeatedSong, volume, subtitle, Random::Range(minPitch, maxPitch));


	//std::string soundToPlay = song;
	//std::string soundChannel = song;
	//bool interupt = true;
	//int volume = 100;
	//SoundManager::getInstance()->PlaySound(soundToPlay, soundChannel, false, interupt, false, volume);



	_lastSong = song;

	return 0;
}

int LuaScriptParser::addCoin(lua_State * lua)
{
	std::cout << "You've picked up a coin! " << std::endl;
	Hud::getInstance()->addCoin(Random::Range(40, 60));
	return 1;
}

int LuaScriptParser::setFishHintText(lua_State * lua)
{
	Hud::getInstance()->setHintText(lua_tostring(lua, -1));
	return 0;
}

int LuaScriptParser::message(lua_State * lua) {

	int amount = lua_tonumber(lua, -1);

	if (amount > 0)
	{
		for (int i = 0; i < amount; i++)
		{
			_messageBoxManager->addToQueue(lua_tostring(lua, -2));
		}
	}
	else
	{
		_currentText = lua_tostring(lua, -1);
	}

	return 0;
}

int LuaScriptParser::visit(lua_State * lua)
{
	currentFunction = lua_tostring(lua, -1);
	return 0;
}

void LuaScriptParser::clearPrintTest(OnCollisionArgs onCollisionArgs)
{
	std::cout << "Clear this shit" << std::endl;
	Hud::getInstance()->setInteractionText("");
}

void LuaScriptParser::resetLevel()
{
	//std::cout << "reset objects1" << std::endl;
	//if (LuaParser::groups.find("Treasure") != LuaParser::groups.end()) {
	//	std::cout << "reset objects2" << std::endl;
	//	for each(GameObject* gameObject in LuaParser::groups["Treasure"])
	//	{
	//		std::cout << "reset objects3" << std::endl;
	//		gameObject->getMaterial()->allowedToRender = true;
	//		World::physics->addCollisionObject(gameObject->getBehaviour<BoxCollider>());
	//		World::physics->addCollisionObject(gameObject->getBehaviour<SphereCollider>());
	//	}
	//}

	for each(GameObject* gameObject in _destoyedObjects) {
		std::cout << "reset treasure" << std::endl;
		gameObject->getMaterial()->allowedToRender = true;
		for each(Collider* col in gameObject->getBehavioursOfType<Collider>())
			World::physics->addCollisionObject(col);
	}
	for each(std::string group in _destroyedGroups)
		for each(GameObject* gameObject in LuaParser::groups[group]) {
			std::cout << "reset grouped object" << std::endl;
			if (gameObject->getMaterial())
				gameObject->getMaterial()->allowedToRender = true;
			if (gameObject->getBehaviour<Collider>())
				World::physics->addCollisionObject(gameObject->getBehaviour<Collider>());
		}

	//DISABLE SPAWNED OBJECTS
	for each(GameObject* gameObject in _spawnedObjects) {
		std::cout << "reset treasure" << std::endl;
		gameObject->getMaterial()->allowedToRender = false;
		for each(Collider* col in gameObject->getBehavioursOfType<Collider>())
			World::physics->removeCollisionObject(col);
	}
	for each(std::string group in _spawnedGroups)
		for each(GameObject* gameObject in LuaParser::groups[group]) {
			std::cout << "reset grouped object" << std::endl;
			if (gameObject->getMaterial())
				gameObject->getMaterial()->allowedToRender = false;
			if (gameObject->getBehaviour<Collider>())
				World::physics->removeCollisionObject(gameObject->getBehaviour<Collider>());
		}
	//DISABLE SPAWNED OBJECTS

	_destoyedObjects.clear();
	_destroyedGroups.clear();
	_spawnedObjects.clear();
	_spawnedGroups.clear();
}

void LuaScriptParser::printTest(OnCollisionArgs onCollisionArgs)
{
	//dynamic cast naar abstractbehaviour
	GameObject* sender = dynamic_cast<AbstractBehaviour*>(onCollisionArgs.sender)->getOwner();
	std::string NewFunction = "on" + sender->getName() + "Collision";

	lua_getglobal(lua, NewFunction.c_str());

	if (lua_isnil(lua, -1)) { //if is doesn't exist, bail out
		lua_settop(lua, 0);
		return;
	}
	bool ePressed = Input::getKey(sf::Keyboard::E);
	lua_pushinteger(lua, (size_t)sender);
	lua_pushboolean(lua, ePressed);
	lua_call(lua, 2, 0);

	//std::cout << onCollisionArgs.collidingWith->getowner
}

int LuaScriptParser::addItemToInventory(lua_State * lua)
{
	int objectPointer = lua_tointeger(lua, -1);
	GameObject* gameObject = (GameObject*)objectPointer;
	Hud::getInstance()->getInventory().addItem(gameObject->getName() + ".png");
	return 0;
}

int LuaScriptParser::removeItemFromInventory(lua_State * lua)
{
	std::string objectName = lua_tostring(lua, -1);
	Hud::getInstance()->getInventory().removeItem(objectName);
	return 0;
}

int LuaScriptParser::setInteractionText(lua_State * lua)
{
	Hud::getInstance()->setInteractionText(lua_tostring(lua, -1));
	return 0;
}

int LuaScriptParser::setSubtitleText(lua_State * lua)
{
	Hud::getInstance()->setSubtitleText(lua_tostring(lua, -2), 2);
	return 0;
}


int LuaScriptParser::showObjectiveDistance(lua_State * lua)
{
	int ListIndex = lua_tonumber(lua, -1);

	int distance = glm::distance(_objectives->at(ListIndex)->getWorldPosition(), _player->getWorldPosition());

	std::string result = "Distance to relic: " + std::to_string(distance);

	if (distance < 350)
		result = "You are close to the objective!";

	_objectiveText = result;

	return 0;
}

void LuaScriptParser::SetPlayerAndObjectives(GameObject* pGameobject, std::vector<GameObject*> * pObjectives)
{
	_player = pGameobject;
	_objectives = pObjectives;
}

void LuaScriptParser::step()
{
	lua_getglobal(lua, currentFunction);

	_messageBoxManager->drawDirectly(_currentText);
	_currentText = "";
	_messageBoxManager->drawObjective(_objectiveText);
	_objectiveText = "";

	if (lua_isnil(lua, -1)) { //if is doesn't exist, bail out
		lua_settop(lua, 0);
		return;
	}

	lua_call(lua, 0, 0);
}
int LuaScriptParser::destroy(lua_State *lua)
{
	int objectPointer = lua_tointeger(lua, -1);
	GameObject* gameObject = (GameObject*)objectPointer;
	std::cout << "DESTROYING OBJECT: " << gameObject->getName() << std::endl;
	//gameObject->setParent(NULL);
	gameObject->getMaterial()->allowedToRender = false;
	for each(Collider* col in gameObject->getBehavioursOfType<Collider>())
		World::physics->removeCollisionObject(col);
	_destoyedObjects.push_back(gameObject);
	//delete gameObject;
	return 1;
}

int LuaScriptParser::destroyGroup(lua_State *lua)
{
	std::string groupName = lua_tostring(lua, -2);
	bool registerDestroy = lua_tostring(lua, -1);
	destroyGroup(groupName, registerDestroy);
	destroyGroup(groupName + "collider", registerDestroy);
	return 0;
}

void LuaScriptParser::destroyGroup(std::string groupName, bool registerDestroy)
{
	if (LuaParser::groups.find(groupName) != LuaParser::groups.end())
	{
		std::cout << "Attempting to destroy group: " << groupName << std::endl;
		for each (GameObject* gameObject in LuaParser::groups[groupName])
		{
			if (gameObject->getMaterial())
				gameObject->getMaterial()->allowedToRender = false;
			if (gameObject->getBehaviour<Collider>())
				World::physics->removeCollisionObject(gameObject->getBehaviour<Collider>());
		}
		std::cout << "Destroyed group: " << groupName << std::endl;
		if (registerDestroy)
			_destroyedGroups.push_back(groupName);
	}
}

int LuaScriptParser::spawnGroup(lua_State *lua)
{
	std::string groupName = lua_tostring(lua, -2);
	bool registerSpawn = lua_tostring(lua, -1);
	spawnGroup(groupName, registerSpawn);
	spawnGroup(groupName + "collider", registerSpawn);
	return 0;
}

void LuaScriptParser::spawnGroup(std::string groupName, bool registerSpawn)
{
	if (LuaParser::groups.find(groupName) != LuaParser::groups.end())
	{
		std::cout << "Attempting to spawn group: " << groupName << std::endl;
		for each (GameObject* gameObject in LuaParser::groups[groupName])
		{
			if (gameObject->getMaterial())
				gameObject->getMaterial()->allowedToRender = true;
			if (gameObject->getBehaviour<Collider>())
				World::physics->addCollisionObject(gameObject->getBehaviour<Collider>());
		}
		std::cout << "Spawned group: " << groupName << std::endl;
		if (registerSpawn)
			_spawnedGroups.push_back(groupName);
	}
}