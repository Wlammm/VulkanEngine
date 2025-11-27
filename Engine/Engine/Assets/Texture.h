#pragma once
#include "Engine/AssetRegistry/AssetRegistry2.h"

class VulkanImage;

struct ImageData
{
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

class Texture : public Asset2
{
public:
    ~Texture();
    
    void LoadPropertiesFromSource() override;
    
    void PostPropertiesSerialized() override;

    static constexpr bool IsExternalAsset() { return true; }
    
    VulkanImage* GetImage() const;
    vk::ImageView GetImageView() const;

    uint GetBindlessIndex() const;

private:
    void GenerateMipLevels(vk::CommandBuffer inCommandBuffer);
    
private:
    VulkanImage* myImage = nullptr;
    
    friend class TextureSystem;
    uint myBindlessIndex;
    
    META(SerializeField)
    ImageData myImageData;
};
