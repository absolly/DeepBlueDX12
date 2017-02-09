#include "Input.h"

bool Input::_mousePressed[sf::Mouse::ButtonCount];
bool Input::_mouseReleased[sf::Mouse::ButtonCount];
bool Input::_mouseDown[sf::Mouse::ButtonCount];

bool Input::getMousePressed(const sf::Mouse::Button mouseIndex)
{
	return _mousePressed[mouseIndex];
}
bool Input::getMouseReleased(const sf::Mouse::Button mouseIndex)
{
	return _mouseReleased[mouseIndex];
}
bool Input::getMouseDown(const sf::Mouse::Button mouseIndex)
{
	return _mouseDown[mouseIndex];
}

void Input::resetMouseButtonsPressed()
{
	for (int i = 0; i < sf::Mouse::ButtonCount; i++)
		_mousePressed[i] = false;
}

void Input::resetMouseButtonsReleased()
{
	for (int i = 0; i < sf::Mouse::ButtonCount; i++)
		_mouseReleased[i] = false;
}

void Input::setMouseDown(const sf::Mouse::Button mouseIndex, const bool mouseDown)
{
	_mouseDown[mouseIndex] = mouseDown;
}
