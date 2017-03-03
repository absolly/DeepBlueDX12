#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include "Content\Inventory\Inventory.h"
#include "mge\materials\HUDMaterial.hpp"

class HudSprite;

class Hud
{
	public:
		static Hud* getInstance();

		Hud( sf::RenderWindow * aWindow );
		virtual ~Hud();
		void draw();

		void drawImage(int x, int y, HudSprite& hudSprite);
		void drawText(int x, int y, std::string* text);

		void setDebugInfo (std::string pInfo);

		void setOxygenLeft(std::string oxygenLeft);
		void setDepth(std::string depthInCM);

	private:
		static Hud* _instance;

		sf::RenderWindow * _window;

		HUDMaterial* _hudMaterial;
        std::string _debugInfo;
		Inventory& _inventory;
		HudSprite _visor;

		HudSprite _depthBar;
		sf::Text _depthText;
		HudSprite _oxygenBar;
		sf::Text _oxygenText;

		bool _noOxygenLeft;
		float _deathSpriteOpacity = 0;
		HudSprite _deathSprite;

        sf::Font _font;
        sf::Text _debugText;

        void _createDebugHud();

		Hud(const Hud&);
		Hud& operator=(const Hud&);

		std::unordered_map<HudSprite*, sf::Sprite*> _stringPointersToSprites;
		std::unordered_map<sf::Sprite*, int> _spriteConnectionCount;
		std::unordered_map<std::string, sf::Sprite*> _spriteCache;
};