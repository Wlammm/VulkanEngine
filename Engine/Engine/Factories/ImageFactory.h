#pragma once

constexpr int FileVersion = 1;

struct ImageData
{
	std::filesystem::path mySourceFile;
	int myWidth;
	int myHeight;
	int myChannels;
	List<unsigned char> myPixelData;
};

class ImageFactory
{
public:
	static class VulkanImage* GetImage(const std::filesystem::path& inPath);

private:
	static class VulkanImage* CreateImageFromImageData(const ImageData& inImageData);

	static void GetImageDataFromImageFile(const std::filesystem::path& inPath, ImageData& out);
	static void GetImageDataFromBinary(const std::filesystem::path& inPath, ImageData& out);
	
	static void SaveImageDataToBinary(const ImageData& inData, const std::filesystem::path& inSavePath);

	static std::filesystem::path GetCachePath(const std::filesystem::path& inFilePath);
};