#include "EnginePch.h"
#include "Filewatcher.h"
#include "Tracy/tracy/Tracy.hpp"

Filewatcher::Filewatcher()
{
	myThread = std::thread(&Filewatcher::WatchThread, this);
}

Filewatcher::~Filewatcher()
{
	myShouldRun = false;
	if(myThread.joinable())
		myThread.join();
}

void Filewatcher::SetShouldRun(const bool inState)
{
	myShouldRun = inState;
}

Filewatcher::CallbackHandle Filewatcher::InsertWatch_TS(const std::filesystem::path& inPath, const std::function<void()> inCallback)
{
	check(std::filesystem::exists(inPath));

	std::unique_lock<std::mutex> lock(myLock);
	if(!myFilesToWatch.contains(inPath))
	{
		FileData data;
		const std::filesystem::file_time_type writeTime = std::filesystem::last_write_time(inPath);
		data.myLastModifiedTime = writeTime;
		myFilesToWatch.insert({ inPath, data });
	}

	CallbackHandle handle = CallbackHandle::NextCallbackID++;
	myFilesToWatch[inPath].myCallbacks.Add(inCallback);
	myFilesToWatch[inPath].myCallbackIDIndexList.Add(handle);
	return handle;
}

void Filewatcher::RemoveWatch_TS(const std::filesystem::path& inPath, const CallbackHandle& inHandle)
{
	std::unique_lock<std::mutex> lock(myLock);

	if (!myFilesToWatch.contains(inPath))
		return;

	int index = myFilesToWatch[inPath].myCallbackIDIndexList.FindIndex(inHandle);

	if (index == -1)
		return;
	
	myFilesToWatch[inPath].myCallbacks.RemoveIndex(index);
	myFilesToWatch[inPath].myCallbackIDIndexList.RemoveIndex(index);

	if (myFilesToWatch[inPath].myCallbacks.IsEmpty())
		myFilesToWatch.erase(inPath);
}

void Filewatcher::FlushChanges()
{
	ZoneScoped;

	List<std::function<void()>> callbacksToFire;
	{
		std::unique_lock<std::mutex> lock(myLock);
		for (FileData* fileData : myModifiedPaths)
		{
			for (const std::function<void()>& callback : fileData->myCallbacks)
				callbacksToFire.Add(callback);
		}
		myModifiedPaths.clear();
	}

	for (const std::function<void()>& callback : callbacksToFire)
		callback();
}

void Filewatcher::WatchThread()
{
	ZoneScoped;
	while(myShouldRun)
	{
		{
			std::unique_lock<std::mutex> lock(myLock);
			for(auto& [path, fileData] : myFilesToWatch)
			{
				if(!std::filesystem::exists(path))
					continue;

				const std::filesystem::file_time_type writeTime = std::filesystem::last_write_time(path);
				if(writeTime != fileData.myLastModifiedTime)
				{
					myModifiedPaths.insert(&fileData);
					fileData.myLastModifiedTime = writeTime;
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
