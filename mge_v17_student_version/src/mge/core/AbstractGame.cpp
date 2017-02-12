#include "AbstractGame.hpp"

#include <iostream>
using namespace std;

#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include <btBulletDynamicsCommon.h>
#include "Content\Core\Input.h"
#include "SFML\Window.hpp"
#include <Windows.h>
#include "Content\Core\EventHandler.h"
#include "mge/config.hpp"


AbstractGame::AbstractGame():_window(NULL),_renderer(NULL),_world(NULL), _fps(0) {
    //ctor

	EventHandler::bindEvent(sf::Event::Closed, this, &AbstractGame::onCloseWindowEvent);
	EventHandler::bindKeyDownEvent(sf::Keyboard::Escape, this, &AbstractGame::onEscapePressedEvent);
	EventHandler::bindKeyDownEvent(sf::Keyboard::F1, this, &AbstractGame::onToggleMouseLock);
}

AbstractGame::~AbstractGame() {
    //dtor
    delete _window;
    delete _renderer;
    delete _world;
}

void AbstractGame::initialize() {
    cout << "Initializing engine..." << endl << endl;
    _initializeWindow();
    _printVersionInfo();
    _initializeGlew();
    _initializeRenderer();
    _initializeWorld();
    _initializeScene();
    cout << endl << "Engine initialized." << endl << endl;
}

///SETUP

void AbstractGame::_initializeWindow() {
    cout << "Initializing window..." << endl;
    _window = new sf::RenderWindow(sf::VideoMode(800, 600), "My Game!", sf::Style::Default, sf::ContextSettings(24,8,0,3,3));    
	
	//_window->setMouseCursorGrabbed(true);
	_window->setMouseCursorVisible(false);
    //_window->setVerticalSyncEnabled(true); 
	SetCursorPos(_window->getPosition().x + _window->getSize().x / 2, _window->getPosition().y + _window->getSize().y / 2);
	//sf::Mouse::setPosition(sf::Vector2i(_window->getPosition().x + _window->getSize().x / 2, _window->getPosition().y + _window->getSize().y / 2));
    cout << "Window initialized." << endl << endl;
}

void AbstractGame::_printVersionInfo() {
    cout << "Context info:" << endl;
    cout << "----------------------------------" << endl;
    //print some debug stats for whoever cares
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    //nice consistency here in the way OpenGl retrieves values
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

    cout << "----------------------------------" << endl << endl;
}

void AbstractGame::_initializeGlew() {
    cout << "Initializing GLEW..." << endl;
    //initialize the opengl extension wrangler
    GLint glewStatus = glewInit();
    cout << "Initialized GLEW, status (1 == OK, 0 == FAILED):" << (glewStatus == GLEW_OK) << endl << endl;
}

void AbstractGame::_initializeRenderer() {
    //setup our own renderer
    cout << "Initializing renderer..." << endl;
    _renderer = new Renderer();
    cout << "Renderer done." << endl << endl;

	_shader = new ShaderProgram();
	_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "Passthrough.vs");
	_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "RenderTexture.fs");
	_shader->finalize();
}

void AbstractGame::_initializeWorld() {
    //setup the world
    cout << "Initializing world..." << endl;
    _world = new World();
    cout << "World initialized." << endl << endl;
}

///LOOP

void AbstractGame::run() {
    sf::Clock updateClock;
    sf::Clock renderClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);

    while (_window->isOpen()) {
        timeSinceLastUpdate += updateClock.restart();

        if (timeSinceLastUpdate > timePerFrame) {
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            while (timeSinceLastUpdate > timePerFrame) {
                timeSinceLastUpdate -= timePerFrame;
                _update(timePerFrame.asSeconds());
                _world->updatePhysics(timePerFrame.asSeconds());
				Input::updateInput();
            }
			
            _render();
            _window->display();

            float timeSinceLastRender = renderClock.restart().asSeconds();
            if (timeSinceLastRender != 0) _fps = 1.0f/timeSinceLastRender;
        }
        _processEvents();
    }
}

void AbstractGame::_update(float pStep) {
    _world->update(pStep);
}

void AbstractGame::_render () {
	// Clear the screen
	
    _renderer->render(_world);

	// The fullscreen quad's FBO
	GLuint quad_VertexArrayID;
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	//// Create and compile our GLSL program from the shaders
	//GLuint quad_programID = LoadShaders("Passthrough.vertexshader", "SimpleTexture.fragmentshader");
	//GLuint texID = glGetUniformLocation(quad_programID, "renderedTexture");
	//GLuint timeID = glGetUniformLocation(quad_programID, "time");


	// Render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1024, 768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader->use();
	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _renderer->renderedTexture);
	glUniform1i(_shader->getUniformLocation("textureDiffuse"), 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);


}

void AbstractGame::_processEvents() {
	EventHandler::handleEvents(*_window); 

	/*sf::Event event;
    bool exit = false;

	Input::updateInput();
	EventHandler::handleEvents(*_window);
	
    //we must empty the event queue
    while( _window->pollEvent( event ) ) {
        //give all system event listeners a chance to handle events
        //optionally to be implemented by you...
        //SystemEventDispatcher::dispatchEvent(event);
		EventHandler::handleEvent(event);
        switch (event.type) {
        case sf::Event::Resized:
            //would be better to move this to the renderer
            //this version implements nonconstrained match viewport scaling
            glViewport(0, 0, event.size.width, event.size.height);
            break;

        default:
            break;
        }
    }
	//SetCursorPos(_window->getPosition().x + _window->getSize().x / 2, _window->getPosition().y + _window->getSize().y / 2);
	//sf::Mouse::setPosition(sf::Vector2i(1920 / 2, 1080 / 2,));
	//sf::Mouse::setPosition(sf::Vector2i(_window->getPosition().x + _window->getSize().x/2, _window->getPosition().y + _window->getSize().y/2));
    if (exit) {
        _window->close();
    }*/
}

void AbstractGame::onCloseWindowEvent(sf::Event & event)
{
	_window->close();
}

void AbstractGame::onEscapePressedEvent(sf::Event::KeyEvent & event)
{
	_window->close();
}

void AbstractGame::onToggleMouseLock(sf::Event::KeyEvent & event)
{
	_mouseCursorVisible = !_mouseCursorVisible;
	_window->setMouseCursorVisible(_mouseCursorVisible);
}
