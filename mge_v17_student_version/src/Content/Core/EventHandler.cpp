#include "EventHandler.h"
#include "SFML\Window.hpp"

std::unordered_map<sf::Event::EventType, Event<sf::Event&>*> EventHandler::_eventMap = std::unordered_map<sf::Event::EventType, Event<sf::Event&>*>();
std::unordered_map<sf::Keyboard::Key, Event<sf::Event::KeyEvent&>*> EventHandler::_keyPressedEventMap = std::unordered_map<sf::Keyboard::Key, Event<sf::Event::KeyEvent&>*>();
std::unordered_map<sf::Keyboard::Key, Event<sf::Event::KeyEvent&>*> EventHandler::_keyReleasedEventMap = std::unordered_map<sf::Keyboard::Key, Event<sf::Event::KeyEvent&>*>();

std::unordered_map<GameObject*, std::vector<EventConnection>> EventHandler::_eventMapConnections = std::unordered_map<GameObject*, std::vector<EventConnection>>();

std::unordered_map<GameObject*, std::vector<KeyEventConnection>> EventHandler::_keyEventMapConnections = std::unordered_map<GameObject*, std::vector<KeyEventConnection>>();

void EventHandler::handleEvents(sf::Window& window)
{
	sf::Event event;
	while (window.pollEvent(event))
		handleEvent(event);
}

void EventHandler::handleEvent(sf::Event& event)
{
	if (_eventMap.find(event.type) != _eventMap.end())
		(*_eventMap[event.type])(event);
	if (event.type == sf::Event::KeyPressed)
		if (_keyPressedEventMap.find(event.key.code) != _keyPressedEventMap.end())
		(*_keyPressedEventMap[event.key.code])(event.key);
	else if (event.type == sf::Event::KeyReleased)
		if (_keyReleasedEventMap.find(event.key.code) != _keyReleasedEventMap.end())
		(*_keyReleasedEventMap[event.key.code])(event.key);
}
