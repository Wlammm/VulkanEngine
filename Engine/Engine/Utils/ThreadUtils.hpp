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
		SetThreadDescription(inThreadHandle, inString.c_str());
		HRESULT r;
		r = SetThreadDescription(
			GetCurrentThread(),
			inString.c_str());

		check(SUCCEEDED(r));
	}
};