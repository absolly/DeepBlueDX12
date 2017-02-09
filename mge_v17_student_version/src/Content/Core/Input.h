#pragma once

#include "EventHandler.h"
#include "SFML\Window.hpp"
#include <iostream>
#include <unordered_set>

class Input
{
public:

	static void updateInput();

	//Mouse button input
	static bool getMouseButtonDown(const int mouseIndex);
	static bool getMouseButtonDown(const sf::Mouse::Button mouseIndex);

	static bool getMouseButtonReleased(const int mouseIndex);
	static bool getMouseButtonReleased(const sf::Mouse::Button mouseIndex);

	static bool getMouseButton(const int mouseIndex);
	static bool getMouseButton(const sf::Mouse::Button mouseIndex);

	//Keyboard input
	static bool getKeyDown(const int keyIndex);
	static bool getKeyDown(const sf::Keyboard::Key keyIndex);

	static bool getKeyReleased(const int keyIndex);
	static bool getKeyReleased(const sf::Keyboard::Key keyIndex);

	static bool getKey(const int keyIndex);
	static bool getKey(const sf::Keyboard::Key keyIndex);

	static class _init
	{
	public:
		_init() {
			//Mouse input
			EventHandler::bindEvent(sf::Event::MouseButtonPressed, this, &_init::onMouseButtonPressedEvent);
			EventHandler::bindEvent(sf::Event::MouseButtonReleased, this, &_init::onMouseButtonReleasedEvent);
			//Keyboard input
			EventHandler::bindEvent(sf::Event::KeyPressed, this, &_init::onKeyPressedEvent);
			EventHandler::bindEvent(sf::Event::KeyReleased, this, &_init::onKeyReleasedEvent);
		}

		//Mouse input
		void onMouseButtonPressedEvent(sf::Event& event) { Input::setMousePressed(event.mouseButton.button); }
		void onMouseButtonReleasedEvent(sf::Event& event) { Input::setMouseReleased(event.mouseButton.button); }
		//Keyboard input
		void onKeyPressedEvent(sf::Event& event) { Input::setKeyPressed(event.key.code); }
		void onKeyReleasedEvent(sf::Event& event) { Input::setKeyReleased(event.key.code); }

	} _initializer;

private:
	//Mouse input
	static void setMousePressed(const sf::Mouse::Button mouseIndex);
	static void setMouseReleased(const sf::Mouse::Button mouseIndex);
	//Keyboard input
	static void setKeyPressed(const sf::Keyboard::Key keyIndex);
	static void setKeyReleased(const sf::Keyboard::Key keyIndex);

	//Mouse input
	static std::unordered_set<int> Input::_mouseButtonsPressed;
	static std::unordered_set<int> Input::_mouseButtonsReleased;
	//Keyboard input
	static std::unordered_set<int> Input::_keysPressed;
	static std::unordered_set<int> Input::_keysReleased;
};

