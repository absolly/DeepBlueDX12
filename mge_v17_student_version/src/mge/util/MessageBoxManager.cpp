#include <cassert>
#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <SFML/Graphics/Text.hpp>
#include "mge/util/MessageBoxManager.hpp"
#include "mge/config.hpp"
#include <string>

MessageBoxManager::MessageBoxManager( sf::RenderWindow * aWindow ): _window( aWindow ), _debugInfo(), _font(), _debugText()
{
	assert ( _window != NULL );

    if (!_font.loadFromFile(Config::MGE_FONT_PATH+ "arial.ttf")) {
        cout << "Could not load font, exiting..." << endl;
        return;
    }

    _createDebugHud();

}

MessageBoxManager::~MessageBoxManager()
{
	//dtor
}

void MessageBoxManager::_createDebugHud() {
    _debugText.setString("");
    _debugText.setFont(_font);
	_debugText.setCharacterSize(16);
	_debugText.setFillColor(sf::Color::White);

	_objectiveText.setString("");
	_objectiveText.setFont(_font);
	_objectiveText.setCharacterSize(16);
	_objectiveText.setFillColor(sf::Color::White);
	_objectiveText.setPosition(10, 100);
}

void MessageBoxManager::addToQueue(string pInfo) {
	if (messages.size() < 1)
	{
		curTime = MessageTimeFrames;
	}

	messages.push(pInfo);
}

void MessageBoxManager::_checkQueue() {
	
	if (curTime < 0 && messages.size() > 0)
	{
		messages.pop();
		curTime = MessageTimeFrames;
	}

	if (messages.size() > 0)
	{
		_debugText.setString(messages.front());
		_debugText.setPosition(200, 500);
	}

	curTime--;
}

void MessageBoxManager::draw()
{
	//glDisable( GL_CULL_FACE );
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);

    _window->pushGLStates();
	
	if (messages.size() > 0)
	{
		_window->draw(_debugText);
	}
	
	_window->popGLStates();

}

void MessageBoxManager::drawObjective(std::string pText)
{
	_objectiveText.setString(pText);
	

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);

	_window->pushGLStates();
		_window->draw(_objectiveText);
	_window->popGLStates();

}
