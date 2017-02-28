#ifndef DEBUGHUD_H
#define DEBUGHUD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

class HudSprite;

class Hud
{
	public:
		static Hud* getInstance();

		Hud( sf::RenderWindow * aWindow );
		virtual ~Hud();
		void draw();

		void drawImage(int x, int y, std::string* image, bool useSpritesPath = true);
		void drawText(int x, int y, std::string* text);

		void setDebugInfo (std::string pInfo);

	private:
		static Hud* _instance;

		sf::RenderWindow * _window;

        std::string _debugInfo;

        sf::Font _font;
        sf::Text _debugText;

        void _createDebugHud();

		Hud(const Hud&);
		Hud& operator=(const Hud&);

		std::unordered_map<HudSprite*, sf::Sprite*> _stringPointersToSprites;
		std::unordered_map<sf::Sprite*, int> _spriteConnectionCount;
		std::unordered_map<std::string, sf::Sprite*> _spriteCache;
};

#endif // DEBUGHUD_H
