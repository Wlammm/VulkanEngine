#pragma once

class BinaryReader
{
public:
	BinaryReader(const std::filesystem::path& inPath);

	void Read(std::filesystem::path& out)
	{
		size_t size;
		Read(size);

	}

	template<typename T>
	void Read(List<T>& out)
	{
		size_t size;
		Read(size);

		out.Resize(size);
		memcpy(out.data(), myBuffer.data(), sizeof(T) * size);
		myBuffer.RemoveRange(sizeof(T) * size);
	}

	template<typename T>
	void Read(T& out)
	{
		memcpy(&out, myBuffer.data(), sizeof(T));
		myBuffer.RemoveRange(sizeof(T));
	}

private:
	List<unsigned char> myBuffer;
};