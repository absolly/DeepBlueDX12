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
	_inventorySlotBackgroundSprite.setOrigin(backgroundSlotBounds.width / 2, backgroundSlotBounds.height / 2);
	_inventorySlotBackgroundSprite.scale(Config::HUD_SCALE_FACTOR);

	Config::updateValue("InventorySlots", _inventorySlots);
	Config::updateValue("InventorySlotSpacing", _inventorySlotSpacing);

	addItem("ShipItem.png");
	addItem("ShipItem.png");
	addItem("ShipItem.png");
	addItem("poison.png");
	removeItem("ShipItem.png");
	addItem("poison.png");
	removeItem("ShipItem.png");
	addItem("ShipItem.png");
	addItem("ShipItem.png");
}

Inventory::~Inventory()
{
}

void Inventory::addItem(std::string itemName)
{
	_inventory.push_back(HudSprite());
	_inventory[_inventory.size() - 1].setSpriteName(itemName);
	sf::FloatRect& itemBounds = _inventory[_inventory.size() - 1].getLocalBounds();
	_inventory[_inventory.size() - 1].setOrigin(itemBounds.width / 2, itemBounds.height / 2);
}

bool Inventory::hasItem(std::string itemName)
{
	return getItemIndex(itemName) != -1;
}

int Inventory::getItemIndex(std::string itemName)
{
	for (int i = _inventory.size() - 1; i > 0; i--)
	{
		if (_inventory[i].getSpriteName() == itemName)
		{
			return i;
		}
	}
	return -1;
}

void Inventory::removeItem(std::string itemName)
{
	removeItem(getItemIndex(itemName));
}

void Inventory::removeItem(int itemIndex)
{
	_inventory.erase(_inventory.begin() + itemIndex);
}

void Inventory::draw()
{
	float windowWidth = _window.getSize().x;
	float windowHeight = _window.getSize().y;

	//_inventoryBackgroundSprite.setPosition(windowWidth/2, windowHeight);
	//_window.draw(_inventoryBackgroundSprite);

	float offset = _inventorySlotBackgroundSprite.getGlobalBounds().width + _inventorySlotSpacing;
	float inventorySlotHeight = _inventorySlotBackgroundSprite.getGlobalBounds().height;
	float totalWidth = (_inventorySlots - 1) * offset;
	for (int i = 0; i < _inventorySlots; i++)
	{
		_inventorySlotBackgroundSprite.setPosition(windowWidth / 2 - (totalWidth / 2) + i * offset, windowHeight - inventorySlotHeight / 2);
		_window.draw(_inventorySlotBackgroundSprite);

		if (i < _inventory.size())
		{
			_inventory[i].setPosition(windowWidth / 2 - (totalWidth / 2) + i * offset, windowHeight - inventorySlotHeight / 2);
			_window.draw(_inventory[i]);
		}
	}
}
