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
#include "Time.h"

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

sf::RenderWindow * AbstractGame::getRenderWindow()
{
	return _window;
}

void AbstractGame::initialize(HINSTANCE pHinstance, HINSTANCE pPrevInstance, int pShowCmd) {
    cout << "Initializing engine..." << endl << endl;

	_initializeWindow(pHinstance, pShowCmd, false);
    _printVersionInfo();
    _initializeAPI();
    _initializeRenderer();
    _initializeWorld();
    _initializeScene();

	_menu = new Menu(_window);


    cout << endl << "Engine initialized." << endl << endl;
}

///SETUP
#ifdef API_OPENGL
bool AbstractGame::_initializeWindow(HINSTANCE hInstance, int ShowCmd, bool fullscreen) {
    cout << "Initializing window..." << endl;
    _window = new sf::RenderWindow(sf::VideoMode((int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y), "My Game!", Config::FULL_SCREEN ? sf::Style::Fullscreen : sf::Style::None, sf::ContextSettings(24,8,0,3,3));

	//_window->setMouseCursorGrabbed(true);
	//_window->setMouseCursorVisible(false);
    //_window->setVerticalSyncEnabled(true); 
	SetCursorPos(_window->getPosition().x + _window->getSize().x / 2, _window->getPosition().y + _window->getSize().y / 2);
	//sf::Mouse::setPosition(sf::Vector2i(_window->getPosition().x + _window->getSize().x / 2, _window->getPosition().y + _window->getSize().y / 2));
    cout << "Window initialized." << endl << endl;
}
#elif defined(API_DIRECTX12)
bool AbstractGame::_initializeWindow(HINSTANCE hInstance, int ShowCmd, bool fullscreen) {
	if (fullscreen) {
		HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hmon, &mi);

		Config::SCREEN_RESOLUTION.x = mi.rcMonitor.right - mi.rcMonitor.left;
		Config::SCREEN_RESOLUTION.y = mi.rcMonitor.bottom - mi.rcMonitor.top;
	}

	WNDCLASSEX windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = NULL;
	windowClass.cbWndExtra = NULL;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = WindowName;
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&windowClass)) {
		MessageBox(NULL, L"Failed to register window class", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hwnd = CreateWindowEx(
		NULL,
		WindowName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd) {
		MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	if (fullscreen) {
		SetWindowLong(hwnd, GWL_STYLE, 0);
	}

	ShowWindow(hwnd, ShowCmd);
	UpdateWindow(hwnd);

	return true;
}
#endif

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

void AbstractGame::_initializeAPI() {
    cout << "Initializing GLEW..." << endl;
    //initialize the opengl extension wrangler
    GLint glewStatus = glewInit();
    cout << "Initialized GLEW, status (1 == OK, 0 == FAILED):" << (glewStatus == GLEW_OK) << endl << endl;
}

void AbstractGame::_initializeRenderer() {
	//return;
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

	_skyboxShader = new ShaderProgram();
	_skyboxShader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH + "Skybox.vs");
	_skyboxShader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH + "Skybox.fs");
	_skyboxShader->finalize();

	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);


	vector<std::string> faces;
	faces.push_back((Config::MGE_TEXTURE_PATH + "TropicalSunnyDay/TropicalSunnyDayRight2048.png"));
	faces.push_back(Config::MGE_TEXTURE_PATH + "TropicalSunnyDay/TropicalSunnyDayLeft2048.png");
	faces.push_back(Config::MGE_TEXTURE_PATH + "TropicalSunnyDay/TropicalSunnyDayUp2048.png");
	faces.push_back(Config::MGE_TEXTURE_PATH + "TropicalSunnyDay/TropicalSunnyDayDown2048.png");
	faces.push_back(Config::MGE_TEXTURE_PATH + "TropicalSunnyDay/TropicalSunnyDayBack2048.png");
	faces.push_back(Config::MGE_TEXTURE_PATH + "TropicalSunnyDay/TropicalSunnyDayFront2048.png");
	cubemapTexture = loadCubemap(faces);

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
    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	_timeSinceStart = sf::Time::Zero;


    while (_window->isOpen()) {
        timeSinceLastUpdate += updateClock.restart();
		_timeSinceStart += updateClock.restart();

		if (timeSinceLastUpdate > timePerFrame) {

			while (timeSinceLastUpdate > timePerFrame) {
				timeSinceLastUpdate -= timePerFrame;
				if (!_GamePaused)
				{
					_update(timePerFrame.asSeconds());
					_world->updatePhysics(timePerFrame.asSeconds());
				}
				Time::DeltaTime = timePerFrame.asSeconds();
				Input::updateInput();
			}

			_render();

			if (_GamePaused)
				_GamePaused = _menu->RenderMenu();
			
			_window->display();
			
            float timeSinceLastRender = renderClock.restart().asSeconds();
			Time::RenderDeltaTime = timeSinceLastRender;
            if (timeSinceLastRender != 0) _fps = 1.0f/timeSinceLastRender;
        }
        _processEvents();
    }
}

void AbstractGame::_update(float pStep) {
    _world->update(pStep);
}

void AbstractGame::ToggleGamePaused(bool pPaused) {
	_GamePaused = pPaused;
}

void AbstractGame::_render () {
	//return;
	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-600, 600, -600, 600, -800, 800);
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
	glViewport(0, 0, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	glDepthMask(GL_FALSE);
	_skyboxShader->use();
	glm::mat4 view = glm::mat4(glm::mat3(glm::inverse(_world->getMainCamera()->getWorldTransform())));
	glUniformMatrix4fv(_skyboxShader->getUniformLocation("view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(_skyboxShader->getUniformLocation("projection"), 1, false, glm::value_ptr(_world->getMainCamera()->getProjection()));
	// ... Set view and projection matrix
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glUniform1i(_skyboxShader->getUniformLocation("skybox"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	// ... Draw rest of the scene
    _renderer->render(_world);
}

void AbstractGame::_renderToQuad() {
	//return;
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
	//glViewport(0, 0, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y); // Render on the whole framebuffer, complete from the lower left corner to the upper right

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
	//setup texture slot 4
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _renderer->waterMaskTexture);
	glUniform1i(_shader->getUniformLocation("waterMaskTexture"), 4);

	glUniform1f(_shader->getUniformLocation("_time"), _timeSinceStart.asSeconds());
	glm::vec3 test = glm::eulerAngles(glm::rotate(glm::quat_cast(_world->getMainCamera()->getWorldTransform()), glm::radians(-90.9f), glm::vec3(1,0,0)));
	glUniform1f(_shader->getUniformLocation("pitch"), glm::degrees(glm::clamp(test.x, 0.f, 180.f)));
	DrawQuad();

}

void AbstractGame::DrawQuad() {
	//return;
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

GLuint AbstractGame::loadCubemap(vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	
	sf::Image image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{		
		if (image.loadFromFile(faces[i])) {
			//create a wrapper for the id (texture is nothing more than that) and
			//load corresponding data into opengl using this id
			//Texture * texture = new Texture();
			std::cout << "loading: " << faces[i] << std::endl;
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr()
			);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
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


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
				DestroyWindow(hwnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

}