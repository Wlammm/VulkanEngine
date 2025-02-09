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
};