#pragma once

class String 
{
public:
	static std::string ToString(const std::wstring& inString)
	{
		std::string str;
		size_t size;
		str.resize(inString.length());
		wcstombs_s(&size, &str[0], str.size() + 1, inString.c_str(), inString.size());
		return str;
	}

	static std::wstring ToWString(const std::string& inString)
	{
		return { inString.begin(), inString.end() };
	}
};