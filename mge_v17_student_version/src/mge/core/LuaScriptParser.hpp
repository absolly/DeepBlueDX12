
#ifndef LuaScriptParser_hpp
#define LuaScriptParser_hpp

#include "mge/util/MessageBoxManager.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <lua.hpp>
#include <string>
#include <vector>

//------------------------------------------------------------------------------------------------------------
//                                                      LuaParser
//------------------------------------------------------------------------------------------------------------
class LuaScriptParser {

public:
	LuaScriptParser(const char* pFileName, sf::RenderWindow * aWindow);
	void SetPlayerAndObjectives(GameObject* pGameobject, std::vector<GameObject*> * pObjectives);
	void step();
	bool resetclick = false;
	void printTest(OnCollisionArgs onCollisionArgs);

private:
	lua_State * lua;
	const char * currentFunction = "";
	MessageBoxManager * _messageBoxManager;
	GameObject * _player;
	std::vector<GameObject*> * _objectives;


	
	//export to lua
	int message(lua_State * lua);
	int showObjectiveDistance(lua_State * lua);
	void runFunctionOnce(std::string pFunction);
	void setup(lua_State * lua);
	int visit(lua_State * lua);
	int playContiniousSound(lua_State * lua);
	int playSound(lua_State * lua);
	void inilua(const char* pFileName);

};

//------------------------------------------------------------------------------------------------------------
//                                                      dispatch()
//------------------------------------------------------------------------------------------------------------
template <int (LuaScriptParser::*func)(lua_State * L)>
int dispatch(lua_State * L) {
	LuaScriptParser * ptr = *static_cast<LuaScriptParser**>(lua_getextraspace(L)); //reference to class instance in lua extraspace
	return ((*ptr).*func)(L); //call member function and return its result
}

#endif /* LuaParser_hpp */