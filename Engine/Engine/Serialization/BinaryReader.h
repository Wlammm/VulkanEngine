#pragma once

class BinaryReader
{
public:
	BinaryReader(const std::filesystem::path& inPath);

	void Read(std::filesystem::path& out)
	{
		size_t size;
		Read(size);
		wchar_t* chars = (wchar_t*)(myBuffer.data() + myReadOffset);
		out = std::wstring(chars, size);
		myReadOffset += sizeof(wchar_t) * size;
	}

	template<typename T>
	void Read(List<T>& out)
	{
		size_t size;
		Read(size);

		out.Resize(size);
		memcpy(out.data(), myBuffer.data() + myReadOffset, sizeof(T) * size);

		myReadOffset += sizeof(T) * size;
	}

	template<typename T>
	void Read(T& out)
	{
		memcpy(&out, myBuffer.data() + myReadOffset, sizeof(T));
		myReadOffset += sizeof(T);
	}

private:
	List<unsigned char> myBuffer;
	size_t myReadOffset = 0;
};