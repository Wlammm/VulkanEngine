#include "EnginePch.h"
#include "EventObserver.h"
#include "Engine/Engine.h"
#include "EventHandler.h"

EventObserver::EventObserver()
{
	Engine::GetEventHandler().AddObserver(this);
}

EventObserver::~EventObserver()
{
	Engine::GetEventHandler().RemoveObserver(this);
}

void EventObserver::SubscribeToEvent(const EventType inType, const std::function<void()> inCallback)
{
	check(!myEvents.contains(inType));
	myEvents[inType] = inCallback;
}

void EventObserver::CallEventIfSubscribed(const EventType inType)
{
	if(myEvents.contains(inType))
		myEvents[inType]();
}
