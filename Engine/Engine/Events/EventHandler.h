#pragma once

enum class EventType;

class EventHandler
{
public:
	EventHandler() = default;

	void FireEvent(const EventType inType);

private:
	friend class EventObserver;
	void AddObserver(class EventObserver* inObserver);
	void RemoveObserver(class EventObserver* inObserver);

private:
	List<class EventObserver*> myObservers{};
	
};