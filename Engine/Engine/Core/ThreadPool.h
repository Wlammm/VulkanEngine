#pragma once

class ThreadPool
{
public:
	ThreadPool(const int inThreadCount = -1);
	~ThreadPool();

	template<typename F>
	void QueueTask(const F& inFunction)
	{
		myLookingForTaskMutex.lock();
		myTasks.push(inFunction);
		myLookingForTaskMutex.unlock();

		myWaitingForTaskCondition.notify_one();
	}

	template<typename F, typename ...Args>
	void QueueTask(const F& inFunction, const Args&... inArgs)
	{
		return QueueTask([inFunction, inArgs...]()
		{
			inFunction(inArgs...);
		});
	}

	template<typename F, typename... Args>
	std::future<typename std::invoke_result<F, Args...>::type> QueueTaskFuture(const F& inFunction, const Args&... inArgs)
	{
		using ReturnType = typename std::invoke_result<F, Args...>::type;

		auto promise = std::make_shared<std::promise<ReturnType>>();

		auto future = promise->get_future();

		QueueTask([promise, inFunction, inArgs...]()
		{
			if constexpr (std::is_same<ReturnType, void>::value)
			{
				inFunction(inArgs...);
				promise->set_value();
			}
			else
			{
				promise->set_value(inFunction(inArgs...));
			}
		});
		return future;
	}

private:
	void LookingForTask();
	void StopAllThreads();

private:
	List<std::thread> myThreads;

	std::mutex myLookingForTaskMutex;
	std::condition_variable myWaitingForTaskCondition;

	std::atomic_bool myShouldExit = false;

	std::queue<std::function<void()>> myTasks;

private:
	inline static uint NumThreadsUsed = 0;
};