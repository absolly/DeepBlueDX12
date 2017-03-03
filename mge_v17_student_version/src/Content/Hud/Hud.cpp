#include <cassert>
#include <iostream>
#include "Hud.hpp"
#include "HudSprite.h"

#include <GL/glew.h>
#include <SFML/Graphics/Text.hpp>
#include "mge/config.hpp"
#include "mge\core\Time.h"

Hud* Hud::_instance;
Hud* Hud::getInstance()
{
	return _instance;
}

Hud::Hud(sf::RenderWindow * window) :
	_window(window),
	_debugInfo(),
	_font(),
	_debugText(),
	_inventory(*new Inventory(*window)),
	_hudMaterial(new HUDMaterial()),
	_visor("Visor.png"),
	_depthBar("DepthBar.png"),
	_oxygenBar("OxygenBar.png")
{
	_instance = this;
	assert(_window != NULL);

	if (!_font.loadFromFile(Config::MGE_FONT_PATH + "arial.ttf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}

	_visor.setScale(Config::HUD_SCALE_FACTOR);
	_depthBar.setScale(Config::HUD_SCALE_FACTOR);
	_oxygenBar.setScale(Config::HUD_SCALE_FACTOR);

	_depthBar.setPosition(50, window->getSize().y - 200);
	_depthText.setPosition(50, window->getSize().y - 200);
	_oxygenBar.setPosition(50, window->getSize().y - 300);

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

	_oxygenText.setString("100%");
	_oxygenText.setFont(_font);
	_oxygenText.setCharacterSize(38);
	_oxygenText.setFillColor(sf::Color::White);

	_depthText.setString("1");
	_depthText.setFont(_font);
	_depthText.setCharacterSize(37);
	_depthText.setFillColor(sf::Color::White);
}

void Hud::setDebugInfo(std::string pInfo) {
	_debugText.setString(pInfo);
	_debugText.setPosition(10, 10);
}

void Hud::setOxygenLeft(std::string oxygenLeft)
{
	if (oxygenLeft == "100") 
	{
		_noOxygenLeft = false;
	}
	if (oxygenLeft == "0" || _noOxygenLeft)
	{
		oxygenLeft = "0";
		_noOxygenLeft = true;
	}

	_oxygenText.setString(oxygenLeft + "%");
	_oxygenText.setPosition(_oxygenBar.getPosition().x + 120 * Config::HUD_SCALE_FACTOR.x - _oxygenText.getLocalBounds().width / 2, _oxygenBar.getPosition().y);
}

void Hud::setDepth(std::string depth)
{
	_depthText.setString(depth);
	_depthText.setPosition(_depthBar.getPosition().x + 64 * Config::HUD_SCALE_FACTOR.x - _depthText.getLocalBounds().width / 2, _depthBar.getPosition().y);
}

Inventory& Hud::getInventory()
{
	return _inventory;
}

void Hud::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);

	_window->pushGLStates();

	_window->draw(_visor);
	_window->draw(_oxygenBar);
	_window->draw(_oxygenText);
	_window->draw(_depthBar);
	_window->draw(_depthText);
	_inventory.draw();

	if (_noOxygenLeft || _deathSpriteOpacity != 0)
	{
		_deathSpriteOpacity += (_noOxygenLeft ? 32 : -128) * Time::DeltaTime;
		if (_deathSpriteOpacity > 255) _deathSpriteOpacity = 255;
		if (_deathSpriteOpacity < 0) _deathSpriteOpacity = 0;
		sf::RectangleShape deathScreen = sf::RectangleShape(sf::Vector2f(_window->getSize()));
		deathScreen.setFillColor(sf::Color(0, 0, 0, _deathSpriteOpacity));
		_window->draw(deathScreen);
	}

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
