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

//------------------------------------------------------------------------------------------------------------
//                                                      LuaParser()
//------------------------------------------------------------------------------------------------------------
LuaScriptParser::LuaScriptParser(const char* pFileName, sf::RenderWindow * aWindow, SoundManager * pSoundManager)
{
	_messageBoxManager = new MessageBoxManager(aWindow);
	_soundManager = pSoundManager;

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
}

void LuaScriptParser::setSoundManager(SoundManager * pSoundManager)
{
	_soundManager = pSoundManager;
}

int LuaScriptParser::playSound(lua_State * lua)
{
	std::string song = lua_tostring(lua, -5);
	std::string channel = lua_tostring(lua, -4);
	bool loop = lua_toboolean(lua, -3);
	bool interrupt = lua_toboolean(lua, -2);
	int volume = lua_tonumber(lua, -1);
	

	bool repeatedSong = (_lastSong == song);

	_soundManager->PlaySound(song, channel, loop, interrupt, repeatedSong, volume);
	_lastSong = song;

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

void LuaScriptParser::printTest(OnCollisionArgs onCollisionArgs)
{
	//dynamic cast naar abstractbehaviour
	std::string NewFunction = "on" + dynamic_cast<AbstractBehaviour*>(onCollisionArgs.sender)->getOwner()->getName() + "Collision";

	lua_getglobal(lua, NewFunction.c_str());

	if (lua_isnil(lua, -1)) { //if is doesn't exist, bail out
		lua_settop(lua, 0);
		return;
	}

	lua_call(lua, 0, 0);

	//std::cout << onCollisionArgs.collidingWith->getowner
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


int LuaScriptParser::destroyGroup(lua_State *lua)
{
	std::string groupName = lua_tostring(lua, -1);
	if (LuaParser::groups.find(groupName) == LuaParser::groups.end())
	{
		return 1;
	}
	std::cout << "Attempting to destroy group: " << groupName << std::endl;
	/*if (LuaParser::groups.find(groupName) == LuaParser::groups.end())
	{
		std::cout << "GROUP CANNOT BE FOUND: " << groupName << std::endl;
		return 1;
	}*/
	for each (GameObject* gameObject in LuaParser::groups[groupName])
	{
		RigidBody* rigidBody = gameObject->getBehaviour<RigidBody>();
		if (rigidBody != nullptr)
		{
			World::physics->removeRigidBody(rigidBody);
		}
		Collider* collider = gameObject->getBehaviour<Collider>();
		if (collider != nullptr)
		{
			World::physics->removeCollisionObject(collider);
		}
		delete gameObject;
	}
	std::cout << "Destroyed group: " << groupName << std::endl;
	LuaParser::groups.erase(groupName);
	return 1;
}