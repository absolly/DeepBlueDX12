#pragma once

#include "Content\Hud\HudSprite.h"

class Inventory
{
public:
	Inventory(sf::RenderWindow& renderWindow);
	void updateFromConfig();
	~Inventory();

	void draw(sf::RenderWindow& renderWindow);

private:
	sf::RenderWindow& _window;

	HudSprite _inventoryBackgroundSprite;
	HudSprite _inventorySlotBackgroundSprite;

	int inventorySlots = 5;
};