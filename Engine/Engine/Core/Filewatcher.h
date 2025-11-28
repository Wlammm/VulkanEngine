#pragma once

class Filewatcher
{
public:
	struct CallbackHandle
	{
		int myID;
		inline static int NextCallbackID = 1;

		CallbackHandle()
		{
			myID = -1;
		}

		CallbackHandle(const int& inID)
		{
			myID = inID;
		}
		
		bool IsValid() const
		{
			return myID != -1;
		}

		bool operator==(const CallbackHandle& inOther) const { return myID == inOther.myID; }
	};

	struct FileData
	{
		// These 2 lists must match.
		List<CallbackHandle> myCallbackIDIndexList;
		List<std::function<void()>> myCallbacks;
		std::filesystem::file_time_type myLastModifiedTime;
	};

public:
	Filewatcher();
	~Filewatcher();

	void SetShouldRun(const bool inState);

	CallbackHandle InsertWatch_TS(const std::filesystem::path& inPath, const std::function<void()> inCallback);

	void RemoveWatch_TS(const std::filesystem::path& inPath, const CallbackHandle& inHandle);

	void FlushChanges();

private:
	void WatchThread();

private:
	std::thread myThread;
	std::mutex myLock;
	std::atomic_bool myShouldRun = true;

	std::unordered_map<std::filesystem::path, FileData> myFilesToWatch;
	std::set<FileData*> myModifiedPaths;
};