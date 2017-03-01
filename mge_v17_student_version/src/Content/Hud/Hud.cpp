#include <cassert>
#include <iostream>
#include "Hud.hpp"
#include "HudSprite.h"

#include <GL/glew.h>
#include <SFML/Graphics/Text.hpp>
#include "mge/config.hpp"

Hud* Hud::_instance;
Hud* Hud::getInstance()
{
	return _instance;
}

Hud::Hud( sf::RenderWindow * aWindow ): 
	_window( aWindow ), 
	_debugInfo(), 
	_font(), 
	_debugText(),
	_inventory(*new Inventory(*aWindow)),
	_hudMaterial(new HUDMaterial())
{
	_instance = this;
	assert ( _window != NULL );

    if (!_font.loadFromFile(Config::MGE_FONT_PATH+ "arial.ttf")) {
        std::cout << "Could not load font, exiting..." << std::endl;
        return;
    }

    _createDebugHud();
}

Hud::~Hud()
{
	//dtor
}

void Hud::_createDebugHud() {
    _debugText.setString("");
    _debugText.setFont(_font);
	_debugText.setCharacterSize(16);
	_debugText.setFillColor(sf::Color::White);
}

void Hud::setDebugInfo(std::string pInfo) {
    _debugText.setString(pInfo);
	_debugText.setPosition(10, 10);
}

void Hud::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);
	//_hudMaterial->render();

	_window->pushGLStates();

	_inventory.draw(*_window);
    _window->draw(_debugText);

	_window->popGLStates();

}

void Hud::drawImage(int x, int y, HudSprite& hudSprite)
{	
	hudSprite.setPosition(x, y);
	_window->draw(hudSprite);
}

void Hud::drawText(int x, int y, std::string* text)
{
}
