#include "EnginePch.h"
#include "SystemDispatcher.h"
#include "Engine.h"
#include "Core/ThreadPool.h"

SystemDispatcher::~SystemDispatcher()
{
	for (auto* system : mySystems)
	{
		del(system);
	}
	mySystems.Clear();
	myQueue.Clear();
}

void SystemDispatcher::DispatchSystems()
{
	ThreadPool& threadPool = Engine::GetThreadPool();
	if (myRequireRebuild)
	{
		RebuildSystemsQueue();
	}

	for (auto& entry : myQueue)
	{
		List<std::future<void>> futures;
		for (const auto& tickable : entry.second)
		{
			futures.Add(threadPool.QueueTaskFuture([tickable]()
			{
				tickable->Tick();
			}));
		}

		for (const auto& future : futures)
		{
			future.wait();
		}
	}
}

void SystemDispatcher::RebuildSystemsQueue()
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
			auto& newQueue = myQueue.Emplace();

			for (auto& dependency : system->GetDependencies())
				newQueue.first.insert(dependency);

			newQueue.second.Add(system);
		}
	}
}