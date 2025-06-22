#include "EnginePch.h"
#include "BinaryWriter.h"

BinaryWriter::BinaryWriter(const std::filesystem::path& inPath)
{
	if (!std::filesystem::exists(inPath))
		std::filesystem::create_directories(inPath.parent_path());

	myStream = std::ofstream(inPath, std::ios::binary);
}

void BinaryWriter::Save()
{
	myStream.close();
}