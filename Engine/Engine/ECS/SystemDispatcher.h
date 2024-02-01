#pragma once

#include "EnginePch.h"
#include <future>
#include "System.h"

class SystemDispatcher
{
public:
	~SystemDispatcher();

	template<typename T>
	void AddSystem()
	{
		mySystems.Add(new T());
		myRequireRebuild = true;
	}

	void DispatchSystems();

	template<typename T>
	T* GetSystem()
	{
		for(System* system : mySystems)
		{
			if (T* castedSystem = dynamic_cast<T*>(system))
				return castedSystem;
		}
		return nullptr;
	}

private:
	void RebuildSystemsQueue();

private:
	List<System*> mySystems{};
	List<std::pair<std::unordered_set<std::string>, List<System*>>> myQueue;
	bool myRequireRebuild = false;
};