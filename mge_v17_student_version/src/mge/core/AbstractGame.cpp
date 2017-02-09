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

AbstractGame::AbstractGame():_window(NULL),_renderer(NULL),_world(NULL), _fps(0) {
    //ctor
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


//    ///start of physics setup
//        //setup world settings
//        btBroadphaseInterface* broadphase = new btDbvtBroadphase();
//
//        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
//        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
//
//        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
//
//        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
//
//        dynamicsWorld->setGravity(btVector3(0, -10, 0));
//        //--------------------
//
//        //setup collision shapes
//        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
//
//        btCollisionShape* fallShape = new btSphereShape(1);
//
//
//    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
//    btRigidBody::btRigidBodyConstructionInfo
//    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
//    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
//    dynamicsWorld->addRigidBody(groundRigidBody);
//
//
//    btDefaultMotionState* fallMotionState =
//        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
//    btScalar mass = 1;
//    btVector3 fallInertia(0, 0, 0);
//    fallShape->calculateLocalInertia(mass, fallInertia);
//    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
//    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
//    dynamicsWorld->addRigidBody(fallRigidBody);
//    ///-----------------------

    while (_window->isOpen()) {
        timeSinceLastUpdate += updateClock.restart();

        if (timeSinceLastUpdate > timePerFrame) {
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            while (timeSinceLastUpdate > timePerFrame) {
                timeSinceLastUpdate -= timePerFrame;
                _update(timePerFrame.asSeconds());
                _world->updatePhysics(timePerFrame.asSeconds());
            }

            _render();
            _window->display();

            float timeSinceLastRender = renderClock.restart().asSeconds();
            if (timeSinceLastRender != 0) _fps = 1.0f/timeSinceLastRender;
        }

        _processEvents();
    }

//    dynamicsWorld->removeRigidBody(fallRigidBody);
//    delete fallRigidBody->getMotionState();
//    delete fallRigidBody;
//
//    dynamicsWorld->removeRigidBody(groundRigidBody);
//    delete groundRigidBody->getMotionState();
//    delete groundRigidBody;
//
//
//    delete fallShape;
//
//    delete groundShape;
//
//
//    delete dynamicsWorld;
//    delete solver;
//    delete collisionConfiguration;
//    delete dispatcher;
//    delete broadphase;
}

void AbstractGame::_update(float pStep) {
    _world->update(pStep);
}

void AbstractGame::_render () {
    _renderer->render(_world);
}

void AbstractGame::_processEvents() {
    sf::Event event;
    bool exit = false;
	Input::mouseMotion = sf::Vector2i();
    //we must empty the event queue
    while( _window->pollEvent( event ) ) {
        //give all system event listeners a chance to handle events
        //optionally to be implemented by you...
        //SystemEventDispatcher::dispatchEvent(event);
		EventHandler::handleEvent(event);
        switch (event.type) {
        case sf::Event::Closed:
            exit = true;
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                exit = true;
            }
            break;
        case sf::Event::Resized:
            //would be better to move this to the renderer
            //this version implements nonconstrained match viewport scaling
            glViewport(0, 0, event.size.width, event.size.height);
            break;
		case sf::Event::MouseMoved:
			//Input::mouseMotion += sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
			//std::cout << "Mouse moved: " << event.mouseMove .x << ", " << event.mouseMove.y << std::endl;
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
    }
}



