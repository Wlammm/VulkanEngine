#pragma once

#include "EnginePch.h"
#include <future>
#include "System.h"
#include "Core/EngineDefines.hpp"

class ISystem;

class SystemDispatcher
{
public:
	SystemDispatcher()
	{

	}

	~SystemDispatcher()
	{
		for (auto* system : mySystems)
		{
			del(system);
		}
		mySystems.Clear();
		myQueue.Clear();
	}

	template<typename T>
	void AddSystem()
	{
		mySystems.Add(new T());
		myRequireRebuild = true;
	}

	void DispatchSystems()
	{
		if (myRequireRebuild)
		{
			RebuildSystemsQueue();
		}

		for (auto& entry : myQueue)
		{
			List<std::future<void>> futures;
			for (const auto& tickable : entry.second)
			{
				futures.Add(std::async(&ISystem::Tick, tickable));
			}

			for (const auto& future : futures)
			{
				future.wait();
			}
		}
	}

private:
	void RebuildSystemsQueue()
	{
		myRequireRebuild = false;
		myQueue.Clear();
		for (ISystem* system : mySystems)
		{
			bool foundSuitableQueue = false;
			for (auto& entry : myQueue)
			{
				bool foundCollision = false;
				for (auto& dependency : system->GetDependencies())
				{
					if (entry.first.contains(dependency))
						foundCollision = true;
				}

				if (!foundCollision)
				{
					for (auto& dependency : system->GetDependencies())
					{
						entry.first.insert(dependency);
					}
					entry.second.Add(system);
					foundSuitableQueue = true;
					break;
				}
			}

			if (!foundSuitableQueue)
			{
				auto& newQueue = myQueue.Add();

				for (auto& dependency : system->GetDependencies())
					newQueue.first.insert(dependency);

				newQueue.second.Add(system);
			}
		}
	}

private:
	List<ISystem*> mySystems{};
	List<std::pair<std::unordered_set<std::string>, List<ISystem*>>> myQueue;
	bool myRequireRebuild = false;
};