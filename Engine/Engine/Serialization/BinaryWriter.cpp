#include "EnginePch.h"
#include "BinaryWriter.h"

BinaryWriter::BinaryWriter(const std::filesystem::path& inPath)
{
	myStream = std::ofstream(inPath, std::ios::binary);
}

void BinaryWriter::Save()
{
	myStream.close();
}