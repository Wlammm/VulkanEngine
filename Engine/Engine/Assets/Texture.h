#pragma once
#include "Engine/AssetRegistry/Asset.h"

class VulkanImage;

using CachePath = std::filesystem::path;

struct ImageData
{
    META(SerializeField)
    std::filesystem::path mySourceFile;
    
    META(SerializeField)
    std::filesystem::file_time_type myLastSourceWriteTime;
    
    META(SerializeField)
    int myWidth;

    META(SerializeField)
    int myHeight;

    META(SerializeField)
    int myChannels;

    META(SerializeField)
    uint myNumMipLevels;

    META(SerializeField)
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
    static CachePath ConvertToCachedPath(const std::filesystem::path& inFilePath);
    
    static ImageData LoadImageDataFromSourceFile(const std::filesystem::path& inPath);
    static bool TryLoadImageDataFromCache(const CachePath& inPath, ImageData& outData);
    static void SaveImageDataToCache(const CachePath& inPath, const ImageData& inData);
    

    void InitializeFromImageData(const ImageData& inImageData);

    void GenerateMipLevels(vk::CommandBuffer inCommandBuffer);
    
private:
    VulkanImage* myImage = nullptr;
    
    friend class TextureSystem;
    uint myBindlessIndex;
};
