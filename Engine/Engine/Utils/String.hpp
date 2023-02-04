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

	static bool EqualsIgnoreCase(const std::string& inA, const std::string& inB)
	{
		if (inA.size() != inB.size())
			return false;

		return std::equal(inA.begin(), inA.end(), inB.begin(), inB.end(),
			[](char a, char b)
			{
				return std::tolower(a) == std::tolower(b);
			});
	}

	static std::string& ToLower(std::string& inString)
	{
		std::transform(inString.begin(), inString.end(), inString.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return inString;
	}

	static std::string ToLowerCopy(const std::string& inString)
	{
		std::string result = inString;
		ToLower(result);
		return result;
	}

	static std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
	{
		size_t start = 0;
		size_t end = string.find_first_of(delimiters);

		std::vector<std::string> result;

		while (end <= std::string::npos)
		{
			std::string token = string.substr(start, end - start);
			if (!token.empty())
				result.push_back(token);

			if (end == std::string::npos)
				break;

			start = end + 1;
			end = string.find_first_of(delimiters, start);
		}

		return result;
	}

	static std::vector<std::string> SplitString(const std::string& string, const char delimiter)
	{
		return SplitString(string, std::string(1, delimiter));
	}
};