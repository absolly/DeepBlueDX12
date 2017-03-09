#ifndef ABSTRACTGAME_H
#define ABSTRACTGAME_H

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>
#include "mge/core/ShaderProgram.hpp"
#include <mge/core/Menu.h>
#include "mge/core/Texture.hpp"

using namespace std;

class World;
class Renderer;

/**
 * Defines the basic structure for a game, with defaults already filled in.
 * Your main game loop should run:
 *
 *  initialize
 *  run
 *
 * in order.
 */
class AbstractGame
{
    public:

        AbstractGame();
        virtual ~AbstractGame();

        //creates a window, initializes glew, a renderer and a world instance
        virtual void initialize();
        //run the actual process of updating all objects, rendering them and processing events
        virtual void run();

		sf::RenderWindow * getRenderWindow();

		virtual void _initializeWindow();

    protected:

        //methods above delegate behaviour to the methods below so that you can override it in a subclass

        //initialize sfml rendering context
        //print info about the current driver version etc
        virtual void _printVersionInfo();
        //initialize the extension wrangler
        virtual void _initializeGlew();
        //create our own custom renderer instance
        virtual void _initializeRenderer();
        //initialize a scene root to which we can attach/add objects
        virtual void _initializeWorld();

        //initialize the actual scene, to be done by a subclass
        virtual void _initializeScene() = 0;

        //call update on all game objects in the display root
        virtual void _update(float pStep);
        //render all game objects in the display root
        virtual void _render();
		virtual void _renderToQuad();
        //process any sfml window events (see SystemEventDispatcher/Listener)
        virtual void _processEvents();
		virtual void _setFogGradient(Texture* pGradientTexture);

		virtual GLuint loadCubemap(vector<std::string> faces);

		sf::RenderWindow* _window;  //sfml window to render into
		Renderer* _renderer;        //the renderer class to render the world
		World* _world;              //the root game object that represents our scene
		float _fps;                 //stores the real fps
		void DrawQuad();
		void ToggleGamePaused(bool pPaused);
    private:
        AbstractGame(const AbstractGame&);
        AbstractGame& operator=(const AbstractGame&);
		bool _GamePaused = true;
		Menu * _menu;
		
		void onCloseWindowEvent(sf::Event& event);
		void onEscapePressedEvent(sf::Event::KeyEvent & event);
		void onToggleMouseLock(sf::Event::KeyEvent& event);
		void disableMouseLock(sf::Event& event);
		void enableMousLock(sf::Event& event);
		void setMouseLockEnabled(bool enabled);
		ShaderProgram* _shader;		
		ShaderProgram* _skyboxShader;
		ShaderProgram* _blurShader;
		ShaderProgram* _shadowShader;
		GLuint quad_vertexbuffer;
		Texture* _fogTexure = nullptr;
		GLuint quadVAO = 0;
		GLuint quadVBO;
		GLuint skyboxVAO;
		GLuint skyboxVBO;
		GLuint cubemapTexture;
		sf::Time _timeSinceStart;
};

#endif // ABSTRACTGAME_H
