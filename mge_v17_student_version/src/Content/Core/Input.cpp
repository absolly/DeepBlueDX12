#include "Input.h"

Input::_init Input::_initializer;

std::unordered_set<int> Input::_mouseButtonsPressed;
std::unordered_set<int> Input::_mouseButtonsReleased;
std::unordered_set<int> Input::_keysPressed;
std::unordered_set<int> Input::_keysReleased;

void Input::updateInput()
{
	_mouseButtonsPressed.clear();
	_mouseButtonsReleased.clear();
	_keysPressed.clear();
	_keysReleased.clear();
}

//Mouse input
bool Input::getMouseButtonDown(const int mouseIndex) { return _mouseButtonsPressed.find(mouseIndex) != _mouseButtonsPressed.end(); } 
bool Input::getMouseButtonDown(const sf::Mouse::Button mouseButton) { return getMouseButtonDown((int)mouseButton); }

bool Input::getMouseButtonReleased(const int mouseIndex) { return _mouseButtonsReleased.find(mouseIndex) != _mouseButtonsReleased.end(); }
bool Input::getMouseButtonReleased(const sf::Mouse::Button mouseButton)  { return getMouseButtonReleased((int)mouseButton); }

bool Input::getMouseButton(const int mouseIndex) { return getMouseButton((sf::Mouse::Button)mouseIndex); }
bool Input::getMouseButton(const sf::Mouse::Button mouseButton) { return sf::Mouse::isButtonPressed(mouseButton); }

//Keyboard input
bool Input::getKeyDown(const int keyIndex) { return _keysPressed.find(keyIndex) != _keysPressed.end(); }
bool Input::getKeyDown(const sf::Keyboard::Key key) { return getKeyDown((int)key); }

bool Input::getKeyReleased(const int keyIndex) { return _keysReleased.find(keyIndex) != _keysReleased.end(); }
bool Input::getKeyReleased(const sf::Keyboard::Key key) { return getKeyReleased((int)key); }

bool Input::getKey(const int keyIndex) { return getKey((sf::Keyboard::Key)keyIndex); }
bool Input::getKey(const sf::Keyboard::Key key) { return sf::Keyboard::isKeyPressed(key); }

//Mouse input
void Input::setMousePressed(const sf::Mouse::Button mouseIndex) { _mouseButtonsPressed.insert(mouseIndex); }
void Input::setMouseReleased(const sf::Mouse::Button mouseIndex) { _mouseButtonsReleased.insert(mouseIndex); }

//Keyboard input
void Input::setKeyPressed(const sf::Keyboard::Key keyIndex) { _keysPressed.insert(keyIndex); }
void Input::setKeyReleased(const sf::Keyboard::Key keyIndex) { _keysReleased.insert(keyIndex); }
