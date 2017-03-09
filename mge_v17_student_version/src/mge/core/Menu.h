#pragma once

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
	void RenderMenu();
private:
	sf::RenderWindow * _window;

	sf::Texture texture;
	sf::Texture texture2;
	sf::Texture texture3;
	sf::Texture texture4;

	sf::Sprite sprite;
	sf::Sprite sprite2;
	sf::Sprite sprite3;
	sf::Sprite sprite4;
};

