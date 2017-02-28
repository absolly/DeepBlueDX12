#pragma once

#include <string>

class HudSprite
{
public:
	HudSprite(std::string spriteName);
	~HudSprite();
private:
	std::string _spriteName;
};