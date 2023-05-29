#pragma once

#include "EventTypes.hpp"

class EventObserver
{
public:
	EventObserver();
	virtual ~EventObserver();

protected:
	void SubscribeToEvent(const EventType inType, const std::function<void()> inCallback);

private:
	friend class EventHandler;
	void CallEventIfSubscribed(const EventType inType);


private:
	std::map<EventType, std::function<void()>> myEvents{};
};