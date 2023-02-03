#pragma once

#define LOG(x) Console::Get().Log(x);
#define LOG_WARNING(x) Console::Get().LogWarning(x);
#define LOG_ERROR(x) Console::Get().LogError(x);

class Console 
{
public:
	Console();
	~Console();
	
	static Console& Get();

	void Log(const std::string& inString);
	void LogWarning(const std::string& inString);
	void LogError(const std::string& inString);

private:
	inline static Console* myInstance = nullptr;

	HANDLE myConsoleHandle;
};