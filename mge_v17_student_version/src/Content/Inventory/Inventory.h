#pragma once

#include "Content\Hud\HudSprite.h"

class Inventory
{
public:
	Inventory(sf::RenderWindow& renderWindow);
	void updateFromConfig();
	~Inventory();

	void addItem(std::string itemName);

	bool hasItem(std::string itemName);

	int getItemIndex(std::string itemName);

	void removeItem(std::string itemName);

	void removeItem(int itemIndex);

	void draw();

private:
	sf::RenderWindow& _window;

	HudSprite _inventoryBackgroundSprite;
	HudSprite _inventorySlotBackgroundSprite;

	std::vector<HudSprite> _inventory;

	int _inventorySlots = 5;
	float _inventorySlotSpacing = 10;
};