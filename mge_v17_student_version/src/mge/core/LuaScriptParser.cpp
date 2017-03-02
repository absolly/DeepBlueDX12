#include "LuaScriptParser.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <mge/behaviours/AbstractBehaviour.hpp>
#include <lua.hpp>
#include <string>

//------------------------------------------------------------------------------------------------------------
//                                                      LuaParser()
//------------------------------------------------------------------------------------------------------------
LuaScriptParser::LuaScriptParser(const char* pFileName, sf::RenderWindow * aWindow)
{
	_messageBoxManager = new MessageBoxManager(aWindow);
	
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

}

int LuaScriptParser::message(lua_State * lua) {
	_currentText = lua_tostring(lua, -1);

	int amount = lua_tonumber(lua, -2);

	if (amount > 0)
	{
		for (int i = 0; i < amount; i++)
		{
			_messageBoxManager->addToQueue(_currentText);
		}
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
	std::string NewFunction = "on" + dynamic_cast<AbstractBehaviour*>(onCollisionArgs.sender)->getOwner()->getName() + "collision";

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

	if (distance < 50)
		result = "The relic is too close to pinpoint!";

	_messageBoxManager->drawObjective(result);

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


	if (lua_isnil(lua, -1)) { //if is doesn't exist, bail out
		lua_settop(lua, 0);
		return;
	}

	lua_call(lua, 0, 0);
}

