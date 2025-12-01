#pragma once
#include "Engine/AssetRegistry/Asset.h"

class VulkanImage;

struct ImageCubeData
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
    List<float> myPixelData;
};

class TextureCube : public Asset
{
public:
    ~TextureCube() override;
    
    List<std::string> GetAssetExtensions() const override { return { ".png", ".hdr"}; }

    bool IsExternalAsset() const override { return true; }
    
    void LoadPropertiesFromSource() override;
    void PostPropertiesSerialized() override;

    vk::ImageView GetImageView() const;

    uint GetBindlessIndex() const;
    
private:
    VulkanImage* myImage = nullptr;

    friend class TextureSystem;
    uint myBindlessIndex = -1;
    
    META(SerializeField)
    ImageCubeData myImageData;
};
