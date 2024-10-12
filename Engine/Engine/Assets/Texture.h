#pragma once
#include "AssetRegistry/Asset.h"

class VulkanImage;

using CachePath = std::filesystem::path;

struct ImageData
{
    std::filesystem::path mySourceFile;
    int myWidth;
    int myHeight;
    int myChannels;
    uint myNumMipLevels;
    List<unsigned char> myPixelData;
};

constexpr int FileVersion = 1;
class Texture : public Asset
{
public:
    Coroutine<void, void, false> Load(const std::filesystem::path inPath) override;
    void Unload() override;

    VulkanImage* GetImage() const;
    vk::ImageView GetImageView() const;

    uint GetBindlessIndex() const;

private:
    static bool IsCached(const std::filesystem::path& inPath);
    static CachePath GetCachePath(const std::filesystem::path& inFilePath);
    
    static ImageData LoadImageDataFromImageFile(const std::filesystem::path& inPath);
    static bool TryLoadImageDataFromCache(const CachePath& inPath, ImageData& out);

    void InitializeFromImageData(const ImageData& inImageData);

    void GenerateMipLevels(vk::CommandBuffer inCommandBuffer);
    
private:
    VulkanImage* myImage = nullptr;
    
    friend class TextureSystem;
    uint myBindlessIndex;
};
