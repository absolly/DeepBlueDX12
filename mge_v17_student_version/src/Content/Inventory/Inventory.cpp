#include "Inventory.h"
#include <iostream>
#include "mge\Config.hpp"

Inventory::Inventory(sf::RenderWindow& renderWindow) :
	_window(renderWindow)
{
	updateFromConfig();
	Config::onConfigUpdated.bind(this, &Inventory::updateFromConfig);
}

void Inventory::updateFromConfig()
{
	std::string inventoryBackground = "InventoryBackground.png";
	Config::updateValue("InventoryBackground", inventoryBackground);
	_inventoryBackgroundSprite.setSpriteName(inventoryBackground);

	sf::FloatRect& backgroundBounds = _inventoryBackgroundSprite.getLocalBounds();
	_inventoryBackgroundSprite.setOrigin(backgroundBounds.width / 2, backgroundBounds.height);

	std::string inventorySlotBackground = "InventorySlotBackground.png";
	Config::updateValue("InventorySlotBackground", inventorySlotBackground);
	_inventorySlotBackgroundSprite.setSpriteName(inventorySlotBackground);

	sf::FloatRect& backgroundSlotBounds = _inventorySlotBackgroundSprite.getLocalBounds();
	_inventorySlotBackgroundSprite.setOrigin(0, backgroundSlotBounds.height);
}

Inventory::~Inventory()
{
}

void Inventory::draw(sf::RenderWindow& renderWindow)
{
	float windowWidth = _window.getSize().x;
	float windowHeight = _window.getSize().y;

	_inventoryBackgroundSprite.setPosition(windowWidth/2, windowHeight);
	_window.draw(_inventoryBackgroundSprite);

	float offset = 110;
	float amount = 5;
	for (int i = 0; i < amount; i++)
	{
		_inventorySlotBackgroundSprite.setPosition(windowWidth / 2 - (amount/2 * offset) + i * offset, windowHeight);
		_window.draw(_inventorySlotBackgroundSprite);
	}
}
