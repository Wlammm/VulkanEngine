#include "EnginePch.h"
#include "BinaryReader.h"

BinaryReader::BinaryReader(const std::filesystem::path& inPath)
{
	check(std::filesystem::exists(inPath));

	std::ifstream stream = std::ifstream(inPath, std::ios::binary);

	// temporary use of std::vector since List does not have istreambuf_iterator implemented as constructor yet. Should be fine since char is copyable.
	myBuffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(stream), {});
	stream.close();
}
