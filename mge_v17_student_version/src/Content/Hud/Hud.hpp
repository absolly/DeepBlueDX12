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
		void setInteractionText(std::string text);
		void setHintText(std::string text);
		void setSubtitleText(std::string text, float counter = 100);
		void draw();

		void drawImage(int x, int y, HudSprite& hudSprite);
		void drawText(int x, int y, std::string* text);

		void setDebugInfo (std::string pInfo);

		void setOxygenLeft(std::string oxygenLeft);
		void setDepth(int depthInCM);

		Inventory& getInventory();
		bool isPlayerKilled = false;

		void addCoin(int pAmount);
		void setCoinCount(int pAmount);
		int getCoinCount();
		bool getNoOxygenLeft();
		void setAbilityStatus(int pStatus);

		void setGameCompleted();
	private:
		void reloadHUD();
		static Hud* _instance;

		sf::RenderWindow * _window;
		int _coins = 0;
		float _coinsDisplayed = 0;

		HUDMaterial* _hudMaterial;
        std::string _debugInfo;
		Inventory& _inventory;
		HudSprite _visor;

		float _depthSpriteOpacity = 0;
		HudSprite _depthBar;
		sf::Text _depthText;
		HudSprite _oxygenBar;
		sf::Text _oxygenText;
		HudSprite _coinCounterBar;
		sf::Text _coinCounterText;
		HudSprite _abilityFish;
		HudSprite _abilityInactive;
		HudSprite _abilityActive;
		sf::Text _interactionText;
		float _subtitleTextTimer;
		sf::Text _subtitleText;
		sf::Text _hintText;
		bool _noOxygenLeft;
		float _deathSpriteOpacity = 0;
		float _gameCompletedOpacity = -100;
		bool _gameCompleted = false;
		HudSprite _deathSprite;
		int _abilityStatus = 1;
        sf::Font _font;
        sf::Text _debugText;

        void _createDebugHud();

		Hud(const Hud&);
		Hud& operator=(const Hud&);

		std::unordered_map<HudSprite*, sf::Sprite*> _stringPointersToSprites;
		std::unordered_map<sf::Sprite*, int> _spriteConnectionCount;
		std::unordered_map<std::string, sf::Sprite*> _spriteCache;
};