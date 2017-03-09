#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>

#include <mge/core/World.hpp>
#include <SFML\Graphics.hpp>

class Menu
{
public:
	Menu(sf::RenderWindow * pWindow);
	~Menu();
	bool RenderMenu();
private:
	sf::RenderWindow * _window;

	sf::Texture texture;
	sf::Texture texture3;
	sf::Texture texture4;
	sf::Texture texture5;

	sf::Sprite sprite;
	sf::Sprite sprite3;
	sf::Sprite sprite4;
	sf::Sprite sprite5;
};

#endif

