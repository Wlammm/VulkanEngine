#include "EnginePch.h"
#include "EventHandler.h"
#include "EventObserver.h"
#include "EventTypes.hpp"

void EventHandler::FireEvent(const EventType inType)
{
	for (EventObserver* ob : myObservers)
		ob->CallEventIfSubscribed(inType);
}

void EventHandler::AddObserver(EventObserver* inObserver)
{
	myObservers.Add(inObserver);
}

void EventHandler::RemoveObserver(EventObserver* inObserver)
{
	myObservers.Remove(inObserver);
}
