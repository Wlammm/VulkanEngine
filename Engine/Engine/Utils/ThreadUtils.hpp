#pragma once

class ThreadUtils
{
public:
	static void NameThread(std::thread& inThread, const std::wstring& inString)
	{
		SetThreadDescription(inThread.native_handle(), inString.c_str());
		HRESULT r;
		r = SetThreadDescription(
			GetCurrentThread(),
			inString.c_str());

		check(SUCCEEDED(r));
	}
};