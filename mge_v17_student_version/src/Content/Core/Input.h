#pragma once

#include "SFML\Window.hpp"

class Input
{
public:
	static bool getMousePressed(const sf::Mouse::Button mouseIndex);
	static bool getMouseReleased(const sf::Mouse::Button mouseIndex);
	static bool getMouseDown(const sf::Mouse::Button mouseIndex);

	static void resetMouseButtonsPressed();
	static void resetMouseButtonsReleased();
	static void setMouseDown(sf::Mouse::Button mouseIndex, bool mouseDown);

private:
	static bool _mousePressed[];
	static bool _mouseReleased[];
	static bool _mouseDown[];
};