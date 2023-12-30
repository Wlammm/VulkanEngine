#include "EnginePch.h"
#include "BinaryReader.h"

BinaryReader::BinaryReader(const std::filesystem::path& inPath)
{
	check(std::filesystem::exists(inPath));

	const size_t fileSize = std::filesystem::file_size(inPath);
	check(fileSize < std::numeric_limits<int>::max());
	myBuffer.Resize(static_cast<int>(fileSize));

	FILE* file;
	errno_t error = fopen_s(&file, inPath.string().c_str(), "rb");
	check(!error);

	fread(myBuffer.data(), sizeof(char), fileSize, file);
	fclose(file);
}
