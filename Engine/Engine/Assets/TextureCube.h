#pragma once
#include "Engine/AssetRegistry/Asset.h"

class VulkanImage;

class TextureCube : public Asset
{
public:
    Coroutine<void, void, false> Load(const std::filesystem::path inPath) override;
    void Unload() override;

    vk::ImageView GetImageView() const;

    uint GetBindlessIndex() const;
    
private:
    VulkanImage* myImage = nullptr;

    friend class TextureSystem;
    uint myBindlessIndex = -1;
};
