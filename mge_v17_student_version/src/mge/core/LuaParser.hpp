
#ifndef LuaParser_hpp
#define LuaParser_hpp

#include <stdio.h>
#include <iostream>
#include <lua.hpp>
#include <SFML/Graphics.hpp>
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/core/Camera.hpp"
#include "mge/behaviours/CameraOrbitBehaviour.hpp"
#include "mge/core/LuaScriptParser.hpp"

#include "mge/core/world.hpp"
#include "mge/core/Mesh.hpp"
#include "mge\core\Physics\RigidBody.hpp"
#include <string>


//------------------------------------------------------------------------------------------------------------
//                                                      LuaParser
//------------------------------------------------------------------------------------------------------------
class LuaParser {


    public:
        LuaParser(World* pWorld);
        void loadFile(const char* pFileName);
        void update(float pStep);
        bool hitTest(GameObject* objectA, GameObject* objectB);
		LuaScriptParser * scriptParser;
		void setPlayerRigidBody(RigidBody& pRigidBody);


    private:
        struct collisionListener {
            //constructor
            collisionListener(GameObject* pObjectA, GameObject* pObjectB, const char* pEventListener) : objectA(pObjectA), objectB(pObjectB), eventListener(pEventListener){};

            GameObject* objectA;
            GameObject* objectB;
            const char* eventListener;
        };
        lua_State * lua;
        //export to lua
		GameObject* _currentGameObject;
		Light* _currentLight;
        int write(lua_State * lua);
        int createObject(lua_State * lua);
        int setMainCharacter(lua_State * lua);
        int addlistener(lua_State * lua);
		int addMaterial(lua_State * lua);
		int addSphereCollider(lua_State * lua);
		int addBoxCollider(lua_State * lua);
		int addMeshCollider(lua_State * lua);
		int createTrigger(lua_State * lua);
		int createLight(lua_State * lua);
		int createFish(lua_State * lua);
		int addLightAttributes(lua_State * lua);
        int destoryLuaObject(lua_State * lua);

		RigidBody * _playerRigidBody;
		Mesh* smallFish;
		AbstractMaterial * gpuinstancing;
        vector<collisionListener> collisionListeners;
        World* _world;
        Mesh* cubeMeshF;
        AbstractMaterial* textureMaterial2;
        map<std::string, glm::vec3> colors;
};

//------------------------------------------------------------------------------------------------------------
//                                                      dispatch()
//------------------------------------------------------------------------------------------------------------
template <int (LuaParser::*func)(lua_State * L)>
int dispatch(lua_State * L) {
    LuaParser * ptr = *static_cast<LuaParser**>(lua_getextraspace(L)); //reference to class instance in lua extraspace
    return ((*ptr).*func)(L); //call member function and return its result
}

#endif /* LuaParser_hpp */
