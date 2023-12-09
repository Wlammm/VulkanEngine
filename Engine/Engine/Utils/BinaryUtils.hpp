#pragma once

class BinaryUtils
{
public:
	template<typename T>
	static void Save(const T& inValue, std::ofstream& inOutStream)
	{
		char* c = (char*)&inValue;
		size_t size = sizeof(T);
		inOutStream.write(c, size);
	}
};