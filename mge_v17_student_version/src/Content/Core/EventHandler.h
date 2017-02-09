#pragma once
#include <unordered_map>
#include "SFML\Window\Event.hpp"
#include "Event.h"
#include "mge\core\GameObject.hpp"
#include "FastDelegate.h"

typedef fastdelegate::FastDelegate1<sf::Event&> EventDelegate;
typedef fastdelegate::FastDelegate1<sf::Event::KeyEvent&> KeyEventDelegate;

struct EventConnection
{
	EventDelegate* eventDelegate;
	sf::Event::EventType eventType;

	EventConnection(EventDelegate* eventDelegate, sf::Event::EventType eventType) :
		eventDelegate(eventDelegate),
		eventType(eventType)
	{}
};
struct KeyEventConnection
{
	KeyEventDelegate* keyEventDelegate;
	sf::Keyboard::Key key;

	KeyEventConnection(KeyEventDelegate* keyEventDelegate, sf::Keyboard::Key key) :
		keyEventDelegate(keyEventDelegate),
		key(key)
	{}
};

class EventHandler
{

public:

	template <class InstanceType, typename Function >
	static void bindEvent(sf::Event::EventType eventType, InstanceType* instanceType, Function function);

	template <class InstanceType, typename Function >
	static void bindKeyDownEvent(sf::Keyboard::Key key, InstanceType* instanceType, Function function);

	template <class InstanceType, typename Function >
	static void bindKeyReleasedEvent(sf::Keyboard::Key key, InstanceType* instanceType, Function function);

	//template <class GameObject>
	static void unbindEvents(GameObject* instance)
	{
		if (_eventMapConnections.find(instance) != _eventMapConnections.end())
		{
			for each (EventConnection eventConnection in _eventMapConnections[instance])
			{
				if (_eventMap.find(eventConnection.eventType) != _eventMap.end())
					_eventMap[eventConnection.eventType]->unbind(eventConnection.eventDelegate);
			}
			_eventMapConnections.erase(instance);
		}
		if (_keyEventMapConnections.find(instance) != _keyEventMapConnections.end())
		{
			for each (KeyEventConnection eventConnection in _keyEventMapConnections[instance])
			{
				if (_keyPressedEventMap.find(eventConnection.key) != _keyPressedEventMap.end())
					_keyPressedEventMap[eventConnection.key]->unbind(eventConnection.keyEventDelegate);
				if (_keyReleasedEventMap.find(eventConnection.key) != _keyReleasedEventMap.end())
					_keyReleasedEventMap[eventConnection.key]->unbind(eventConnection.keyEventDelegate);
			}
			_keyEventMapConnections.erase(instance);
		}
	}

	static void handleEvents(sf::Window& window);
	static void handleEvent(sf::Event& event);
	
private:

	static std::unordered_map<sf::Event::EventType, Event<sf::Event&>*> _eventMap;

	static std::unordered_map<GameObject*, std::vector<EventConnection>> _eventMapConnections;
	static std::unordered_map<GameObject*, std::vector<KeyEventConnection>> _keyEventMapConnections;

	static std::unordered_map<sf::Keyboard::Key, Event<sf::Event::KeyEvent&>*> _keyPressedEventMap;
	static std::unordered_map<sf::Keyboard::Key, Event<sf::Event::KeyEvent&>*> _keyReleasedEventMap;

};

template <class InstanceType, typename Function >
inline void EventHandler::bindEvent(sf::Event::EventType eventType, InstanceType* instance, Function function)
{
	if (_eventMap.find(eventType) == _eventMap.end()) 
		_eventMap[eventType] = new Event<sf::Event&>();
	EventDelegate* eventDelegate = _eventMap[eventType]->bind(instance, function);
	_eventMapConnections[instance].push_back(EventConnection(eventDelegate, eventType));
}

template <class InstanceType, typename Function >
inline void EventHandler::bindKeyDownEvent(sf::Keyboard::Key key, InstanceType* instance, Function function)
{
	if (_keyPressedEventMap.find(key) == _keyPressedEventMap.end())
		_keyPressedEventMap[key] = new Event<sf::Event::KeyEvent&>();
	KeyEventDelegate* keyEventDelegate = _keyPressedEventMap[key]->bind(instance, function);
	_keyEventMapConnections[instance].push_back(KeyEventConnection(keyEventDelegate, key));
}

template <class InstanceType, typename Function >
inline void EventHandler::bindKeyReleasedEvent(sf::Keyboard::Key key, InstanceType* instance, Function function)
{
	if (_keyReleasedEventMap.find(key) == _keyReleasedEventMap.end())
		_keyReleasedEventMap[key] = new Event<sf::Event::KeyEvent&>();
	KeyEventDelegate* keyEventDelegate = _keyReleasedEventMap[key]->bind(instance, function);
	_keyEventMapConnections[instance].push_back(KeyEventConnection(keyEventDelegate, key));
}	