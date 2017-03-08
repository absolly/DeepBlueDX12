
#ifndef LuaScriptParser_hpp
#define LuaScriptParser_hpp

#include "mge/util/MessageBoxManager.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <mge/core/SoundManager.hpp>
#include <stdio.h>
#include <iostream>
#include <lua.hpp>
#include <string>
#include <vector>
#include <map>

//------------------------------------------------------------------------------------------------------------
//                                                      LuaParser
//------------------------------------------------------------------------------------------------------------
class LuaScriptParser {

public:
	LuaScriptParser(const char* pFileName, sf::RenderWindow * aWindow, SoundManager * pSoundManager);
	void SetPlayerAndObjectives(GameObject* pGameobject, std::vector<GameObject*> * pObjectives);
	void step();
	int destroy(lua_State * lua);
	int destroyGroup(lua_State * lua);
	bool resetclick = false;
	void printTest(OnCollisionArgs onCollisionArgs);
	int setInteractionText(lua_State * lua);
	int setSubtitleText(lua_State * lua);
	void setSoundManager(SoundManager * pSoundManager);

	void clearPrintTest(OnCollisionArgs onCollisionArgs);
private:
	void resetLevel();
	lua_State * lua;
	const char * currentFunction = "";
	MessageBoxManager * _messageBoxManager;
	std::string _objectiveText;
	GameObject * _player;
	std::string _currentText = "";
	std::vector<GameObject*> * _objectives;
	std::vector<GameObject*> _destoyedObjects;
	std::vector<std::string> _destroyedGroups;
	std::string _lastSong;
	bool _BreathingIn = true;
	SoundManager * _soundManager;

	//export to lua
	int message(lua_State * lua);
	int showObjectiveDistance(lua_State * lua);
	void runFunctionOnce(std::string pFunction);
	void setup(lua_State * lua);
	int visit(lua_State * lua);
	int playContiniousSound(lua_State * lua);
	int playSound(lua_State * lua);
	int addCoin(lua_State * lua);
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