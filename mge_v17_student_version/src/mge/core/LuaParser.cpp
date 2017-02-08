#include "LuaParser.hpp"
#include "mge/config.hpp"

//------------------------------------------------------------------------------------------------------------
//                                                      LuaParser()
//------------------------------------------------------------------------------------------------------------
LuaParser::LuaParser(World* pWorld) {
    _world = pWorld;
    lua = luaL_newstate();
    luaL_openlibs(lua);

    *static_cast<LuaParser**>(lua_getextraspace(lua)) = &(*this); //store 'this' in lua extra space

    lua_pushcfunction(lua, &dispatch<&LuaParser::createObject>); //binding to c++ member via 'dispatch' wrapper
    lua_setglobal(lua, "createObject"); //function name in lua
    lua_pushcfunction(lua, &dispatch<&LuaParser::setMainCharacter>);
    lua_setglobal(lua, "setmaincharacter");
    lua_pushcfunction(lua, &dispatch<&LuaParser::addlistener>);
    lua_setglobal(lua, "addlistener");
    lua_pushcfunction(lua, &dispatch<&LuaParser::destoryLuaObject>);
    lua_setglobal(lua, "destroy");


    //load the cube mesh and texture for later use.
    cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_unity.obj");
    textureMaterial2 = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"bricks.jpg"));

    //bind colors to names
    colors["brown"] = glm::vec3(81,21,21);
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
    if(error != 0) {
        std::cout << "stack = " << lua_gettop(lua) << "\n";
        std::cout << "error = " << error << "\n";
        std::cout << "message = " << lua_tostring(lua, -1) << "\n";
        lua_pop(lua, 1);
    }

    //get the start function and execute it
    lua_getglobal(lua, "start");
    if(lua_isnil(lua,-1)) {
        std::cout << "function 'start' does not exist" << std::endl;
        lua_settop(lua, 0);
        return;
    }

    //again check for errors
    error = lua_pcall(lua, 0, 0, 0);
    if(error != 0) {
        std::cout << "stack = " << lua_gettop(lua) << "\n";
        std::cout << "error = " << error << "\n";
        std::cout << "message = " << lua_tostring(lua, -1) << "\n";
        lua_pop(lua, 1);
    }
}

//------------------------------------------------------------------------------------------------------------
//                                                      update()
//                                  Checks for collisions in the list and fires the lua events
//------------------------------------------------------------------------------------------------------------
void LuaParser::update(float pStep) {
    //for-each listener in the collisionListeners
    for(collisionListener listener : collisionListeners) {
        //check if both are still valid(have a parent) and then check if they collide
        if(listener.objectA->getParent() != NULL && listener.objectB->getParent() != NULL && hitTest(listener.objectA, listener.objectB)) {
            //output the event listener name
            std::cout << listener.eventListener << std::endl;

            //call the event listener function in lua
            lua_getglobal(lua, listener.eventListener);
            if(lua_isnil(lua,-1)) {
                std::cout << "function "<< listener.eventListener <<" does not exist" << std::endl;
                lua_settop(lua, 0);
                return;
            }

            //push the two parameters
            lua_pushlightuserdata(lua, listener.objectA);
            lua_pushlightuserdata(lua, listener.objectB);

            //check for errors with protected call
            int error = lua_pcall(lua, 2, 0, 0);
            if(error != 0) {
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
    if(!lua_islightuserdata(lua, -1)) {
        lua_pushstring(lua, "invalid parameter for fucntion 'setMainCharacter', should be of type 'lightuserdata'");
        lua_error(lua);
    }

    //get the gameobject and set the behaviour
    GameObject * mainCharacter = (GameObject*)lua_touserdata(lua, -1);
    mainCharacter->addBehaviour(new KeysBehaviour());

    //create a camera and set its behaviour to oribit the main character
    Camera* camera = new Camera ("camera", glm::vec3(0,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);
    camera->addBehaviour(new CameraOrbitBehaviour (10, 30, 150, 1, mainCharacter));

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
    if(lua_gettop(lua) != 3) {
        lua_pushstring(lua, "invalid parameter count for function 'addlistener'");
        lua_error(lua);
        return 0;
    }

    //check if parameters are of correct type
    if(!lua_islightuserdata(lua, -3)) {
        lua_pushstring(lua, "invalid parameter 1");
        lua_error(lua);
        return 0;
    }
    if(!lua_islightuserdata(lua, -2)) {
        lua_pushstring(lua, "invalid parameter 2");
        lua_error(lua);
        return 0;
    }
    if(!lua_isstring(lua, -1)) {
        lua_pushstring(lua, "invalid parameter 3");
        lua_error(lua);
        return 0;
    }

    //add the listener to the vector
    collisionListeners.push_back(collisionListener((GameObject*)lua_touserdata(lua, -3),(GameObject*)lua_touserdata(lua, -2),lua_tostring(lua, -1)));
    return 0;
}

//------------------------------------------------------------------------------------------------------------
//                                                      createObject()
//                                Create an object from lua, returns lightuserdata to lua
//                                   This function returns the created gameobject to lua
//------------------------------------------------------------------------------------------------------------
int LuaParser::createObject(lua_State * lua) {

    int params = lua_gettop(lua);
    float x;
    float y;
    float z;
    float scaleX = 1;
    float scaleY = 1;
    float scaleZ = 1;
    std::string name;
    std::string colorName;

    //if 8 parameters are given we expect the scale to be included
    if(params == 8) {
        if(!lua_isstring(lua, -8)) {
            lua_pushstring(lua, "invalid parameter 1");
            lua_error(lua);
            return 0;
        }
        if(!lua_isstring(lua, -7)) {
            lua_pushstring(lua, "invalid parameter 2");
            lua_error(lua);
            return 0;
        }
        if(!lua_isstring(lua, -6)) {
            lua_pushstring(lua, "invalid parameter 3");
            lua_error(lua);
            return 0;
        }
        if(!lua_isstring(lua, -5)) {
            lua_pushstring(lua, "invalid parameter 4");
            lua_error(lua);
            return 0;
        }
        if(!lua_isstring(lua, -4)) {
            lua_pushstring(lua, "invalid parameter 5");
            lua_error(lua);
            return 0;
        }
        if(!lua_isstring(lua, -3)) {
            lua_pushstring(lua, "invalid parameter 3");
            lua_error(lua);
            return 0;
            lua_pushstring(lua, "invalid parameter 6");
            lua_error(lua);
            return 0;
        }
        if(!lua_isstring(lua, -2)) {
            lua_pushstring(lua, "invalid parameter 7");
            lua_error(lua);
            return 0;
        }
        if(!lua_isstring(lua, -1)) {
            lua_pushstring(lua, "invalid parameter 8");
            lua_error(lua);
            return 0;
        }

        name = lua_tostring(lua,-8);
        x = lua_tonumber(lua, -7);
        y = lua_tonumber(lua, -6);
        z = lua_tonumber(lua, -5);
        scaleX = lua_tonumber(lua, -4);
        scaleY = lua_tonumber(lua, -3);
        scaleZ = lua_tonumber(lua, -2);
        colorName = lua_tostring(lua, -1);

    }
    //if 5 parameters where given then we expect the scale to be (1,1,1)
    else if(params == 5) {
        if(!lua_isstring(lua, -5)) {
            lua_pushstring(lua, "invalid parameter 1");
            lua_error(lua);
            return 0;
        }
        if(!lua_isnumber(lua, -4)) {
            lua_pushstring(lua, "invalid parameter 2");
            lua_error(lua);
            return 0;
        }
        if(!lua_isnumber(lua, -3)) {
            lua_pushstring(lua, "invalid parameter 3");
            lua_error(lua);
            return 0;
        }
        if(!lua_isnumber(lua, -2)) {
            lua_pushstring(lua, "invalid parameter 4");
            lua_error(lua);
            return 0;
        }
        if(!lua_isstring(lua, -1)) {
            lua_pushstring(lua, "invalid parameter 5");
            lua_error(lua);
            return 0;
        }
        name = lua_tostring(lua,-5);
        x = lua_tonumber(lua, -4);
        y = lua_tonumber(lua, -3);
        z = lua_tonumber(lua, -2);
        colorName = lua_tostring(lua, -1);
    } else {
        lua_pushstring(lua, "invalid parameter count for function 'createObject'");
        lua_error(lua);
        return 0;
    }

    //create color material with color defined in the table, the color materials are created here since we might want to transition to color codes later.
    ColorMaterial* colorMat = new ColorMaterial(glm::normalize(colors[colorName]));

    //print the objects debug info to the console
    std::cout << "name: " << name << "\tpos: " << x << "," << y << "," << -z << "\t" << "scale: " << scaleX << "," << scaleY << "," << scaleZ << "\t" << "color: " << colorName << std::endl;

    //create the gameobject, the z is negative to represent unity's axis system
    GameObject* go = new GameObject(name, glm::vec3(x,y,-z));
    go->scale(glm::vec3(scaleX,scaleY,scaleZ));
    go->setMesh (cubeMeshF);
    go->setMaterial(colorMat);
    _world->add(go);

    //return the object to lua
    lua_pushlightuserdata(lua, go);
    return 1;
}

//------------------------------------------------------------------------------------------------------------
//                                                      destroyLuaObject()
//                             Check if object wasn't destroyed already and then remove/delete it
//                                          This function does not return anything to lua
//------------------------------------------------------------------------------------------------------------
int LuaParser::destoryLuaObject(lua_State * lua) {
    if(!lua_islightuserdata(lua, -1)) {
        lua_pushstring(lua, "invalid parameter for 'destroy'");
        lua_error(lua);
        return 0;
    }

    GameObject* go = (GameObject*)lua_touserdata(lua,-1);
    if(go->getParent() != NULL) {
        _world->remove(go);
        delete(go);
    }

    return 0;
}
