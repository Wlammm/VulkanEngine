#pragma once

#include "EnginePch.h"
#include <future>
#include "System.h"

class ISystem;

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
		for(ISystem* system : mySystems)
		{
			if (T* castedSystem = dynamic_cast<T*>(system))
				return castedSystem;
		}
		return nullptr;
	}

private:
	void RebuildSystemsQueue();

private:
	List<ISystem*> mySystems{};
	List<std::pair<std::unordered_set<std::string>, List<ISystem*>>> myQueue;
	bool myRequireRebuild = false;
};