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
	_inventoryBackgroundSprite.setSpriteName(Config::getConfigValue("InventoryBackground", "InventoryBackground.png"));
	sf::FloatRect& backgroundBounds = _inventoryBackgroundSprite.getLocalBounds();
	_inventoryBackgroundSprite.setOrigin(backgroundBounds.width / 2, backgroundBounds.height);
	_inventoryBackgroundSprite.scale(Config::HUD_SCALE_FACTOR);

	_inventorySlotBackgroundSprite.setSpriteName(Config::getConfigValue("InventorySlotBackground", "InventorySlotBackground.png"));
	sf::FloatRect& backgroundSlotBounds = _inventorySlotBackgroundSprite.getLocalBounds();
	_inventorySlotBackgroundSprite.setOrigin(backgroundSlotBounds.width/2, backgroundSlotBounds.height);
	_inventorySlotBackgroundSprite.scale(Config::HUD_SCALE_FACTOR);

	Config::updateValue("InventorySlots", _inventorySlots);
	Config::updateValue("InventorySlotSpacing", _inventorySlotSpacing);
}

Inventory::~Inventory()
{
}

void Inventory::draw()
{
	float windowWidth = _window.getSize().x;
	float windowHeight = _window.getSize().y;

	_inventoryBackgroundSprite.setPosition(windowWidth/2, windowHeight);
	_window.draw(_inventoryBackgroundSprite);

	float offset = _inventorySlotBackgroundSprite.getGlobalBounds().width + _inventorySlotSpacing;

	float totalWidth = (_inventorySlots-1) * offset;
	for (int i = 0; i < _inventorySlots; i++)
	{
		_inventorySlotBackgroundSprite.setPosition(windowWidth / 2 - (totalWidth/2) + i * offset, windowHeight - 5);
		_window.draw(_inventorySlotBackgroundSprite);
	}
}
