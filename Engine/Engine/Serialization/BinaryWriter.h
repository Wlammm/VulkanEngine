#pragma once

class BinaryWriter
{
public:
	BinaryWriter(const std::filesystem::path& inPath);

	void Save();

	void Write(const std::filesystem::path& inPath)
	{
		std::wstring data = inPath.wstring();
		size_t dataSize = data.length();
		Write(dataSize);

		char* c = (char*)data.c_str();
		myStream.write(c, dataSize * sizeof(wchar_t));
	}

	template<typename T>
	void Write(const List<T>& inList)
	{
		Write(inList.size());
		char* c = (char*)inList.data();
		myStream.write(c, sizeof(T) * inList.size());
	}

	template<typename T>
	void Write(const T& inValue)
	{
		char* c = (char*)&inValue;
		size_t size = sizeof(T);
		myStream.write(c, size);
	}

private:
	std::ofstream myStream;
};