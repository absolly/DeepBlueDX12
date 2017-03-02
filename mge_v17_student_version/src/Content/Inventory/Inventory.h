#pragma once

#include "Content\Hud\HudSprite.h"

class Inventory
{
public:
	Inventory(sf::RenderWindow& renderWindow);
	void updateFromConfig();
	~Inventory();

	void draw();

private:
	sf::RenderWindow& _window;

	HudSprite _inventoryBackgroundSprite;
	HudSprite _inventorySlotBackgroundSprite;

	int _inventorySlots = 5;
	float _inventorySlotSpacing = 10;
};