#include "EnginePch.h"
#include "Filewatcher.h"

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

Filewatcher::CallbackHandle Filewatcher::InsertWatch(const std::filesystem::path& inPath, const std::function<void()> inCallback)
{
	check(std::filesystem::exists(inPath));

	std::unique_lock<std::mutex>(myLock);
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

void Filewatcher::RemoveWatch(const std::filesystem::path& inPath, const CallbackHandle& inHandle)
{
	std::unique_lock<std::mutex>(myLock);

	if (!myFilesToWatch.contains(inPath))
		return;

	size_t index = myFilesToWatch[inPath].myCallbackIDIndexList.FindIndex(inHandle);

	myFilesToWatch[inPath].myCallbacks.RemoveIndex(index);
	myFilesToWatch[inPath].myCallbackIDIndexList.RemoveIndex(index);

	if (myFilesToWatch[inPath].myCallbacks.IsEmpty())
		myFilesToWatch.erase(inPath);
}

void Filewatcher::FlushChanges()
{
	myLock.lock();

	for(FileData* fileData : myModifiedPaths)
	{
		for (const std::function<void()>& callback : fileData->myCallbacks)
			callback();
	}
	myModifiedPaths.clear();

	myLock.unlock();
}

void Filewatcher::WatchThread()
{
	while(myShouldRun)
	{
		for(auto& [path, fileData] : myFilesToWatch)
		{
			if(!std::filesystem::exists(path))
				continue;

			const std::filesystem::file_time_type writeTime = std::filesystem::last_write_time(path);
			if(writeTime != fileData.myLastModifiedTime)
			{
				myLock.lock();
				myModifiedPaths.insert(&fileData);
				fileData.myLastModifiedTime = writeTime;
				myLock.unlock();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
