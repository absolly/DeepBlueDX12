#include <cassert>
#include <iostream>
#include "Hud.hpp"
#include "HudSprite.h"

#include <GL/glew.h>
#include <SFML/Graphics/Text.hpp>
#include "mge/config.hpp"
#include "mge\core\Time.h"
#include "Content\Core\Input.h"
#include "mge\core\LuaParser.hpp"

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
	_depthBar(),
	_oxygenBar(),
	_coinCounterBar(),
	_abilityFish("fish_hud.png"),
	_abilityInactive("InventorySlotBackground.png"),
	_abilityActive("bubble_active.png")
{
	_instance = this;
	assert(_window != NULL);

	if (!_font.loadFromFile(Config::MGE_FONT_PATH + "Simple-regular.ttf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}
	_createDebugHud();

	_visor.setScale(Config::HUD_SCALE_FACTOR);
	_depthBar.setScale(Config::HUD_SCALE_FACTOR);
	_oxygenBar.setScale(Config::HUD_SCALE_FACTOR);
	_coinCounterBar.setScale(Config::HUD_SCALE_FACTOR);
	_abilityFish.setScale(Config::HUD_SCALE_FACTOR);
	_abilityInactive.setScale(Config::HUD_SCALE_FACTOR);
	_abilityActive.setScale(Config::HUD_SCALE_FACTOR);

	_abilityFish.setPosition(70, window->getSize().y - 300);
	_abilityInactive.setPosition(70, window->getSize().y - 300);
	_abilityActive.setPosition(70, window->getSize().y - 300);

	_depthBar.setPosition(50, window->getSize().y - 200);
	_depthText.setPosition(50, window->getSize().y - 200);
	_oxygenBar.setPosition(50, window->getSize().y - 300);
	_coinCounterBar.setPosition(50, window->getSize().y - 400);
	_coinCounterText.setPosition(50, window->getSize().y - 400);
	_interactionText.setPosition(sf::Vector2f((_window->getSize().x / 2) - (_interactionText.getGlobalBounds().width / 2), (_window->getSize().y / 2) - (_interactionText.getGlobalBounds().height / 2)));
	_subtitleText.setPosition((_window->getSize().x / 2) - (_subtitleText.getGlobalBounds().width / 2), _window->getSize().y - 200);


	Config::onConfigUpdated.bind(this, &Hud::reloadHUD);
	reloadHUD();
}

Hud::~Hud()
{
	//dtor
}

void Hud::setInteractionText(std::string text)
{
	_interactionText.setString(text);
	_interactionText.setPosition(sf::Vector2f((_window->getSize().x / 2) - (_interactionText.getGlobalBounds().width / 2), (_window->getSize().y / 2) - (_interactionText.getGlobalBounds().height / 2)));
}

void Hud::setHintText(std::string text)
{
	_hintText.setString(text);
	_hintText.setPosition(sf::Vector2f((_window->getSize().x / 2) - (_hintText.getGlobalBounds().width / 2), (_window->getSize().y / 2) - (_hintText.getGlobalBounds().height / 2)));
}

void Hud::setSubtitleText(std::string text, float timer)
{
	_subtitleTextTimer = timer;
	_subtitleText.setString(text);
	_subtitleText.setPosition((_window->getSize().x / 2) - (_subtitleText.getGlobalBounds().width / 2), _window->getSize().y - 200);
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
	_subtitleText.setOutlineColor(sf::Color::Black);

	_interactionText.setString("");
	_interactionText.setFont(_font);
	_interactionText.setCharacterSize(37);
	_interactionText.setFillColor(sf::Color::White);
	_interactionText.setOutlineColor(sf::Color(12, 19, 28));
	_interactionText.setOutlineThickness(1);

	_hintText.setString("");
	_hintText.setFont(_font);
	_hintText.setCharacterSize(37);
	_hintText.setFillColor(sf::Color::White);
	_hintText.setOutlineColor(sf::Color(12, 19, 28));
	_hintText.setOutlineThickness(1);

	_subtitleText.setString("");
	_subtitleText.setFont(_font);
	_subtitleText.setCharacterSize(25);
	_subtitleText.setFillColor(sf::Color::White);
	_subtitleText.setOutlineColor(sf::Color(12, 19, 28));
	_subtitleText.setOutlineThickness(1);

	_coinCounterText.setString(to_string(_coins));
	_coinCounterText.setFont(_font);
	_coinCounterText.setCharacterSize(37);
	_coinCounterText.setFillColor(sf::Color::White);


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

void Hud::setDepth(int depth)
{
	std::string depthString = to_string(depth);
	_depthSpriteOpacity = depth*1.5f;
	_depthText.setString(depthString);
	_depthText.setPosition(_depthBar.getPosition().x + 64 * Config::HUD_SCALE_FACTOR.x - _depthText.getLocalBounds().width / 2, _depthBar.getPosition().y);
}

Inventory& Hud::getInventory()
{
	return _inventory;
}

void Hud::addCoin(int pAmount)
{
	std::cout << "adding coins: " + pAmount << std::endl;
	_coins += pAmount;
}

void Hud::setCoinCount(int pAmount) {
	_coinsDisplayed = pAmount;
	_coins = pAmount;
}

int Hud::getCoinCount()
{
	return _coins;
}

bool Hud::getNoOxygenLeft()
{
	return _noOxygenLeft;
}

void Hud::setAbilityStatus(int pStatus)
{
	_abilityStatus = pStatus;
}


void Hud::reloadHUD()
{
	float coinx = 50;
	Config::updateValue("coinx", coinx);
	float coiny = 300;
	Config::updateValue("coiny", coiny);
	_coinCounterText.setPosition(coinx, _window->getSize().y - coiny);

	float coinSize = 38;
	Config::updateValue("coinSize", coinSize);
	_coinCounterText.setCharacterSize(coinSize);

	float oxigenx = 50;
	Config::updateValue("oxigenx", oxigenx);
	float oxigeny = 300;
	Config::updateValue("oxigeny", oxigeny);
	_oxygenBar.setPosition(oxigenx, _window->getSize().y - oxigeny);

	float oxigenSize = 38;
	Config::updateValue("oxigenSize", oxigenSize);
	_oxygenText.setCharacterSize(oxigenSize);

	float depthx = 50;
	Config::updateValue("depthx", depthx);
	float depthy = 200;
	Config::updateValue("depthy", depthy);
	_depthBar.setPosition(depthx, _window->getSize().y - depthy);

	float depthSize = 38;
	Config::updateValue("depthSize", depthSize);
	_depthText.setCharacterSize(depthSize);
}

void Hud::draw()
{
	if (Input::getKey(sf::Keyboard::P))
		addCoin(100);
	if (Input::getKey(sf::Keyboard::F7))
		reloadHUD();
	/*if (Input::getKey(sf::Keyboard::R))
	{
		if (LuaParser::groups.find("door1") != LuaParser::groups.end())
		{
			for each (GameObject* gameObject in LuaParser::groups["door1"])
			{
				delete gameObject;
			}
			LuaParser::groups.erase("door1");
			std::cout << "Destroyed group: " << "door1" << std::endl;
		}
		if (LuaParser::groups.find("door2") != LuaParser::groups.end())
		{
			for each (GameObject* gameObject in LuaParser::groups["door2"])
			{
				delete gameObject;
			}
			LuaParser::groups.erase("door2");
			std::cout << "Destroyed group: " << "door2" << std::endl;
		}
	}*/

	if (_subtitleTextTimer > 0)
	{
		_subtitleTextTimer -= Time::RenderDeltaTime;
		if (_subtitleTextTimer <= 0)
		{
			_subtitleTextTimer = 0;
			_subtitleText.setString("");
		}
	}

	if (_coinsDisplayed < _coins)
	{
		_coinsDisplayed += Time::RenderDeltaTime * 20;
		if (_coinsDisplayed > _coins)
			_coinsDisplayed = _coins;
	}
	else if (_coinsDisplayed > _coins)
	{
		_coinsDisplayed -= Time::RenderDeltaTime * 20;
		if (_coinsDisplayed < _coins)
			_coinsDisplayed = _coins;
	}
	_coinCounterText.setString(to_string((int)round(_coinsDisplayed)));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);

	_window->pushGLStates();

	_window->draw(_visor);
	_window->draw(_oxygenBar);
	_window->draw(_oxygenText);
	_window->draw(_depthBar);
	_window->draw(_depthText);
	_window->draw(_coinCounterBar);
	_window->draw(_coinCounterText);
	_window->draw(_interactionText);
	_window->draw(_hintText);
	_window->draw(_subtitleText);
	sf::RectangleShape s;
	_inventory.draw();
	switch (_abilityStatus)
	{
	case 0:
		_window->draw(_abilityFish);
		_window->draw(_abilityActive);
		break;
	case 1:
		_window->draw(_abilityFish);
		_window->draw(_abilityInactive);
		break;
	case 2:
		_window->draw(_abilityInactive);
		break;
	}
	if (!isPlayerKilled && (_noOxygenLeft || _deathSpriteOpacity != 0))
	{
		_deathSpriteOpacity += (_noOxygenLeft ? 96 : -128) * Time::DeltaTime;
		if (_deathSpriteOpacity > 255) _deathSpriteOpacity = 255;
		if (_deathSpriteOpacity < 0) _deathSpriteOpacity = 0;
		sf::RectangleShape deathScreen = sf::RectangleShape(sf::Vector2f(_window->getSize()));
		deathScreen.setFillColor(sf::Color(0, 0, 0, _deathSpriteOpacity));
		_window->draw(deathScreen);
		if (_deathSpriteOpacity >= 255)
			isPlayerKilled = true;
	}
	else if (isPlayerKilled) {
		_deathSpriteOpacity += 512 * Time::DeltaTime;
		if (_deathSpriteOpacity > 255) _deathSpriteOpacity = 255;
		if (_deathSpriteOpacity < 0) _deathSpriteOpacity = 0;
		sf::RectangleShape deathScreen = sf::RectangleShape(sf::Vector2f(_window->getSize()));
		deathScreen.setFillColor(sf::Color(0, 0, 0, _deathSpriteOpacity));
		_window->draw(deathScreen);

		sf::Text _deathText;
		_deathText.setString("You Died, Press E to respawn");
		_deathText.setFont(_font);
		_deathText.setCharacterSize(24);
		_deathText.setFillColor(sf::Color(255, 255, 255, _deathSpriteOpacity));
		_deathText.setPosition(sf::Vector2f((_window->getSize().x / 2) - (_deathText.getGlobalBounds().width / 2), (_window->getSize().y / 2) - (_deathText.getGlobalBounds().height / 2)));
		_window->draw(_deathText);
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
