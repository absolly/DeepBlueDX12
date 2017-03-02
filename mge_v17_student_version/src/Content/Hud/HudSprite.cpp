#include "HudSprite.h"
#include "Hud.hpp"
#include "mge\config.hpp"
#include <exception>
std::unordered_map<std::string, sf::Texture> HudSprite::_textureCache;
std::unordered_map<sf::Texture*, int> HudSprite::_textureReferenceCount;

HudSprite::HudSprite()
{
	_spriteName = "";
	_texture = nullptr;
}
HudSprite::HudSprite(std::string spriteName, bool useSpritePath)
{
	loadTexture(spriteName, useSpritePath);
}

HudSprite::~HudSprite()
{
	removeTextureReference();
}

void HudSprite::setSpriteName(std::string spriteName, bool useSpritePath)
{
	//if (_spriteName != spriteName)
	{
		//New sprite name;
		removeTextureReference();
		loadTexture(spriteName, useSpritePath);
	}
}

void HudSprite::loadTexture(std::string spriteName, bool useSpritePath)
{
	if (useSpritePath)
		spriteName = Config::MGE_SPRITES_PATH + spriteName;
	_spriteName = spriteName;
	if (_textureCache.find(spriteName) == _textureCache.end())
	{
		//If texture isn't stored in the texture cache yet
		_textureCache[spriteName] = sf::Texture();
		

		if (!_textureCache[spriteName].loadFromFile(spriteName))
		{
			//Image not found
			std::string exception = "Image could not be found: " + spriteName;
			throw std::exception(exception.c_str());
		}
		
		_texture = &_textureCache[spriteName];
		setTexture(*_texture, true);
		_textureReferenceCount[_texture] = 1;
	}
	else
	{
		std::cout << "FOUND TEXTTURE" << std::endl;
		//If the texture is stored in the texture cache already
		_texture = &_textureCache[spriteName];
		setTexture(*_texture, true);
		_textureReferenceCount[_texture]++;
	}
}

void HudSprite::removeTextureReference()
{
	if (_texture != nullptr)
	{
		int& textureReferenceCount = _textureReferenceCount[_texture];
		textureReferenceCount--;
		if (textureReferenceCount == 0)
		{
			//No references to the texture anymore.
			_textureReferenceCount.erase(_texture);
			if (_spriteName != "")
				_textureCache.erase(_spriteName);
		}
	}
}
