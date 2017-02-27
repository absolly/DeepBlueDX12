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
#include "mge/core/Camera.hpp"
#include "mge/core/Light.hpp"


AbstractGame::AbstractGame():_window(NULL),_renderer(NULL),_world(NULL), _fps(0) {
    //ctor

	EventHandler::bindEvent(sf::Event::Closed, this, &AbstractGame::onCloseWindowEvent);
	EventHandler::bindKeyDownEvent(sf::Keyboard::Escape, this, &AbstractGame::onEscapePressedEvent);
	EventHandler::bindKeyDownEvent(sf::Keyboard::F1, this, &AbstractGame::onToggleMouseLock);
	EventHandler::bindEvent(sf::Event::LostFocus, this, &AbstractGame::disableMouseLock);
	EventHandler::bindEvent(sf::Event::GainedFocus, this, &AbstractGame::enableMousLock);
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
    _window = new sf::RenderWindow(sf::VideoMode(1600, 900), "My Game!", sf::Style::Default, sf::ContextSettings(24,8,0,3,3));    
	
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
	_shader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH + "Passthrough.vs");
	_shader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH + "RenderTexture.fs");
	_shader->finalize();

	_blurShader = new ShaderProgram();
	_blurShader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH + "Passthrough.vs");
	_blurShader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH + "GaussianBlur.fs");
	_blurShader->finalize();

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};


	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	if (_fogTexure == nullptr)
		_fogTexure = Texture::load(Config::MGE_TEXTURE_PATH + "fog.png");
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
    sf::Time timePerFrame = sf::seconds(1.0f / 120.0f);

    while (_window->isOpen()) {
        timeSinceLastUpdate += updateClock.restart();

		if (timeSinceLastUpdate > timePerFrame) {

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

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-300, 300, -300, 300, -800, 800);
	glm::mat4 depthViewMatrix;
	for (Light* light : World::activeLights) { 
	  if (light->type == Light::DIRECTIONAL) { 
	    depthViewMatrix = glm::inverse(light->getWorldTransform());
	  } 
	} 

	glBindFramebuffer(GL_FRAMEBUFFER, _renderer->ShadowBuffer);
	glViewport(0, 0, 4096, 4096);
	// Render on the whole framebuffer, complete from the lower left corner to the upper right

	// We don't use bias in the shader, but instead we draw back faces, 
	// which are already separated from the front faces by a small distance 
	// (if your geometry is made this way)
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // Cull front-facing triangles -> draw only back-facing triangles

						 // Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_renderer->renderShadowMap(_world, _world->getTransform(), depthViewMatrix, depthProjectionMatrix, true);

	//_shadowShader->use();

	//DrawQuad();

	// Clear the screen
	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _renderer->FramebufferName);
	glViewport(0, 0, 1600, 900); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
    _renderer->render(_world);
}

void AbstractGame::_renderToQuad() {
	glActiveTexture(GL_TEXTURE0);

	GLboolean horizontal = true, first_iteration = true;
	GLuint amount = 10;
	_blurShader->use();
	GLuint iterationID = _blurShader->getUniformLocation("iteration");
	for (GLuint i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _renderer->pingpongFBO[horizontal]);
		glUniform1i(iterationID, i);
		glBindTexture(
			GL_TEXTURE_2D, first_iteration ? _renderer->brightnessTexture : _renderer->pingpongBuffer[!horizontal]
		);
		DrawQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	// Render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, 1600, 900); // Render on the whole framebuffer, complete from the lower left corner to the upper right

								 // Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_shader->use();
	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _renderer->renderedTexture);
	glUniform1i(_shader->getUniformLocation("renderedTexture"), 0);
	//setup texture slot 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _renderer->depthTexture);
	glUniform1i(_shader->getUniformLocation("depthTexture"), 1);
	//setup texture slot 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _renderer->pingpongBuffer[!horizontal]);
	glUniform1i(_shader->getUniformLocation("bloomTexture"), 2);
	//setup texture slot 3
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _fogTexure->getId());
	glUniform1i(_shader->getUniformLocation("fogTexture"), 3);

	DrawQuad();

}

void AbstractGame::DrawQuad() {
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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

void AbstractGame::_setFogGradient(Texture * pGradientFogTexture)
{
	_fogTexure = pGradientFogTexture;
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
	setMouseLockEnabled(!Input::mouseLocked);
}

void AbstractGame::disableMouseLock(sf::Event & event)
{
	setMouseLockEnabled(false);
}

void AbstractGame::enableMousLock(sf::Event & event)
{
	setMouseLockEnabled(true);
}

void AbstractGame::setMouseLockEnabled(bool enabled)
{
	Input::mouseLocked = enabled;
	_window->setMouseCursorVisible(!Input::mouseLocked);

	sf::Vector2i screenResolution = sf::Vector2i(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	sf::Vector2i windowSize = sf::Vector2i(_window->getSize().x, _window->getSize().y);
	sf::Vector2i windowPosition = screenResolution / 2 - windowSize / 2;
	sf::Vector2i windowCenter = windowPosition + windowSize / 2;

	if (Input::mouseLocked)
	{
		sf::Mouse::setPosition(windowCenter);
	}
}
