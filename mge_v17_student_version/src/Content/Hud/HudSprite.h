#pragma once

#include <string>
#include <unordered_map>
#include "SFML\Graphics.hpp"

class HudSprite : public sf::Sprite
{
public:
	HudSprite();
	HudSprite(std::string spriteName, bool useDefaultSpritePath = true);
	~HudSprite();

	void setSpriteName(std::string spriteName, bool useSpritePath = true);

private:
	std::string _spriteName;
	sf::Texture* _texture;

	void loadTexture(std::string spriteName, bool useSpritePath);
	void removeTextureReference();

	static std::unordered_map<std::string, sf::Texture> _textureCache;
	static std::unordered_map<sf::Texture*, int> _textureReferenceCount;
};