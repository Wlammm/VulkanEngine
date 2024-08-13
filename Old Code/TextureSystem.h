#pragma once
#include "ECS/System.h"

class VulkanImage;
class VulkanDescriptorSet;

using TextureHandle = uint;

struct TextureData
{
    uint myIndex;
    TextureHandle myHandle;
};

// https://jorenjoestar.github.io/post/vulkan_bindless_texture/
class TextureSystem : public System
{
public:
    TextureSystem();
    ~TextureSystem();

    TextureHandle AddTexture(VulkanImage* inImage, const vk::Sampler inSampler);

    vk::DescriptorSet GetDescriptorSet() const;
    vk::DescriptorSetLayout GetDescriptorLayout() const;
    
private:
    void CreateDescritorPool();
    void CreateDescriptorSetLayout();
    void CreateDescriptorSet();
    
private:
    std::unordered_map<TextureHandle, TextureData> myTextures;
    
    vk::DescriptorPool myDescriptorPool;
    
    vk::DescriptorSetLayout myDescriptorLayout;
    vk::DescriptorSet myDescriptorSet;

    uint myNextArrayIndex = 0;
    TextureHandle myNextHandle = 0;
};
