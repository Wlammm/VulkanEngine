#include "EnginePch.h"
#include "ThreadPool.h"
#include "Utils/ThreadUtils.hpp"

ThreadPool::ThreadPool(const int inThreadCount)
{
	uint threadsToCreate = inThreadCount;

	if (inThreadCount == -1)
		threadsToCreate = static_cast<uint>(std::thread::hardware_concurrency() * 0.75f);

	NumThreadsUsed += threadsToCreate;
	check(NumThreadsUsed < std::thread::hardware_concurrency());

	LOG("Creating threadpool with %i threads", threadsToCreate);

	for(uint index = 0; index < threadsToCreate; ++index)
	{
		myThreads.Add(std::thread(&ThreadPool::LookingForTask, this));

		std::wstring name = L"ThreadPool worker #" + std::to_wstring(index + 1);
		ThreadUtils::NameThread(myThreads.Last(), name);
	}
}

ThreadPool::~ThreadPool()
{
	StopAllThreads();
	NumThreadsUsed -= static_cast<int>(myThreads.size());
	myThreads.Clear();
}

void ThreadPool::LookingForTask()
{
	while(true)
	{
		std::unique_lock<std::mutex> lock(myLookingForTaskMutex);
		myWaitingForTaskCondition.wait(lock, [&]()
			{
				return !myTasks.empty() || myShouldExit;
			});

		if (myShouldExit)
			break;

		std::function<void()> task = myTasks.front();
		myTasks.pop();

		lock.unlock();
		task();
		lock.lock();
		myWaitingForTaskCondition.notify_all();
	}
}

void ThreadPool::StopAllThreads()
{
	myShouldExit = true;

	myWaitingForTaskCondition.notify_all();

	for(auto& thread : myThreads)
	{
		if (thread.joinable())
			thread.join();
	}
}
