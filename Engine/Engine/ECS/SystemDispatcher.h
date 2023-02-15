#pragma once

#include "EnginePch.h"
#include <future>
#include "System.h"

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
		mySystems.clear();
		myQueue.clear();
	}

	template<typename T>
	void AddSystem()
	{
		mySystems.emplace_back(new T());
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
			std::vector<std::future<void>> futures;
			for (auto& tickable : entry.second)
			{
				futures.emplace_back(std::async(&ISystem::Tick, tickable));
			}

			for (auto& future : futures)
			{
				future.wait();
			}
		}
	}

private:
	void RebuildSystemsQueue()
	{
		myRequireRebuild = false;
		myQueue.clear();
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
					entry.second.emplace_back(system);
					foundSuitableQueue = true;
					break;
				}
			}

			if (!foundSuitableQueue)
			{
				auto& newQueue = myQueue.emplace_back();

				for (auto& dependency : system->GetDependencies())
					newQueue.first.insert(dependency);

				newQueue.second.emplace_back(system);
			}
		}
	}

private:
	std::vector<ISystem*> mySystems{};
	std::vector<std::pair<std::unordered_set<std::string>, std::vector<ISystem*>>> myQueue;
	bool myRequireRebuild = false;
};