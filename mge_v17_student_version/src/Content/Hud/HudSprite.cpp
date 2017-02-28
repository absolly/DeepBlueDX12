#include "HudSprite.h"
#include "Hud.hpp"

HudSprite::HudSprite(std::string spriteName)
{
}

HudSprite::~HudSprite()
{
	Hud::getInstance()->dereference(this);
}
