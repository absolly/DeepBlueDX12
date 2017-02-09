#pragma once
#include <unordered_set>
#include "FastDelegate.h"

template <typename EventArgsType>
class Event
{
	typedef fastdelegate::FastDelegate1<EventArgsType> EventDelegate;
public:
	Event();
	~Event()
	{
		for each (EventDelegate* delegate in _boundDelegates)
		{
			delete delegate;
		}
	}
	template <typename InstanceType, typename Function >
	EventDelegate* bind(InstanceType* instanceType, Function function)
	{
		//If an error 
		//	"fastdelegate::FastDelegate1<EventArgsType,fastdelegate::detail::DefaultVoid>::FastDelegate1': no overloaded function takes 2 arguments
		//Is being thrown. This might mean that you pass along a function that has a pointer to an instance as argument, while the event doesn't, or vise versa.
		//Example: 
		//	void coolEvent(sf::Event);
		//	Event<sf::Event&> event; 
		//	event.bind(this, &coolEvent);
		//
		//	in this case, event.bind expected a function with as argument "sf::Event&", but got one with an argument "sf::Event".
		//
		// Check the stack trace to see where it goes wrong.
		EventDelegate* eventDelegate = new EventDelegate(instanceType, function);
		_boundDelegates.insert(eventDelegate);
		return eventDelegate;
	}

	bool unbind(EventDelegate* eventDelegate)
	{
		if (_boundDelegates.find(eventDelegate) == _boundDelegates.end()) return false;
		if (!_executingDelegates)
		{
			_boundDelegates.erase(eventDelegate);
			delete eventDelegate;
		}
		else
		{
			_delegatesToUnbind.insert(eventDelegate);
		}
		return true;
	}

	void operator()(EventArgsType eventArgs)
	{
		_executingDelegates = true;

		for each (EventDelegate* delegate in _boundDelegates)
		{
			if (_delegatesToUnbind.find(delegate) == _delegatesToUnbind.end())
			{
				(*delegate)(eventArgs);
			}
		}
		_executingDelegates = false;

		for (auto itr = _delegatesToUnbind.begin(); itr != _delegatesToUnbind.end(); ++itr)
		{
			EventDelegate* delegate = *itr;
			_boundDelegates.erase(*itr);
			delete delegate;
		}
		_delegatesToUnbind.clear();
	}

private:
	std::unordered_set<fastdelegate::FastDelegate1<EventArgsType>*> _boundDelegates;
	std::unordered_set<fastdelegate::FastDelegate1<EventArgsType>*> _delegatesToUnbind;

	bool _executingDelegates = false;
};

template<typename EventArgsType>
inline Event<EventArgsType>::Event()
{
	_boundDelegates = std::unordered_set<fastdelegate::FastDelegate1<EventArgsType>*>();
	_delegatesToUnbind = std::unordered_set<fastdelegate::FastDelegate1<EventArgsType>*>();
}
