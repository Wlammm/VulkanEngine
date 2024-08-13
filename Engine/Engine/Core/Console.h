#pragma once

#define LOG(x, ...) Console::Get().Log(x, ##__VA_ARGS__)
#define LOG_WARNING(x, ...) Console::Get().LogWarning(x, ##__VA_ARGS__)
#define LOG_ERROR(x, ...) Console::Get().LogError(x, ##__VA_ARGS__)

class Console 
{
public:
	Console();
	~Console();
	
	static Console& Get();

	void Log(const char* inString, ...);
	void Log(const std::string& inString);

	void LogWarning(const char* inString, ...);
	void LogWarning(const std::string& inString, ...);

	void LogError(const char* inString, ...);
	void LogError(const std::string& inString, ...);

private:
	inline static Console* myInstance = nullptr;

	HANDLE myConsoleHandle;

	bool myLogToFileEnabled = false;
	std::ofstream myLogToFileStream;
	std::streambuf* myCoutBuffer = nullptr;
};