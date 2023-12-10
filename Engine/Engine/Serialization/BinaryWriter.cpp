#include "EnginePch.h"
#include "BinaryWriter.h"

BinaryWriter::BinaryWriter(const std::filesystem::path& inPath)
{
	if (!std::filesystem::exists(inPath))
		std::filesystem::create_directories(inPath.parent_path());

	myStream = std::ofstream(inPath, std::ios::binary);

	if (!myStream.is_open())
		int a = 10;
}

void BinaryWriter::Save()
{
	myStream.close();
}