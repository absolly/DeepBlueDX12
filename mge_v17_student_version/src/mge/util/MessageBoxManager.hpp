#ifndef MESSAGEBOXMANAGER_H
#define MESSAGEBOXMANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <string>

class MessageBoxManager
{
	public:
		MessageBoxManager( sf::RenderWindow * aWindow );
		virtual ~MessageBoxManager();
		void draw();
		void drawObjective(std::string pText);
		void drawDirectly(std::string pText);

		void _checkQueue();
		void addToQueue (std::string pInfo);

	private:
		sf::RenderWindow * _window;
		std::queue<std::string> messages;
        std::string _debugInfo;
		int MessageTimeFrames = 120;
		int curTime = 0;

        sf::Font _font;
        sf::Text _debugText;
		sf::Text _objectiveText;

        void _createDebugHud();

		MessageBoxManager(const MessageBoxManager&);
		MessageBoxManager& operator=(const MessageBoxManager&);

};

#endif // DEBUGHUD_H
