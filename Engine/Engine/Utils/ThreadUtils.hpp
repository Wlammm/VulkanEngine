#pragma once

class ThreadUtils
{
public:
	static void NameThread(std::thread& inThread, const std::wstring& inString)
	{
		NameThread(inThread.native_handle(), inString);
	}

	static void NameThread(HANDLE inThreadHandle, const std::wstring& inString)
	{
		HRESULT result = SetThreadDescription(inThreadHandle, inString.c_str());
		check(SUCCEEDED(result));
	}
	
	static void RegisterThisThreadAsMainThread()
	{
		myMainThreadID = std::this_thread::get_id();
	}
	
	static bool IsOnMainThread()
	{
		return myMainThreadID == std::this_thread::get_id();
	}
	
private:
	inline static std::thread::id myMainThreadID;
};