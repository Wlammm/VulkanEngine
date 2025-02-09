#pragma once
#include "System/System.h"

class Texture;
class VulkanImage;
class VulkanDescriptorSet;

// https://jorenjoestar.github.io/post/vulkan_bindless_texture/
class TextureSystem : public System
{
public:
    TextureSystem();
    ~TextureSystem();

    void Tick();
    
    static void RegisterTexture_TS(Texture* inTexture);

    vk::DescriptorSet GetDescriptorSet() const;
    vk::DescriptorSetLayout GetDescriptorLayout() const;
    
private:
    void RegisterAllQueuedTextures();
    void RegisterTexture(Texture* inTexture);
    
    void CreateDescritorPool();
    void CreateDescriptorSetLayout();
    void CreateDescriptorSet();
    
private:
    inline static MutexList<Texture*> myTextures;
    
    vk::DescriptorPool myDescriptorPool;
    
    inline static vk::DescriptorSetLayout myDescriptorLayout;
    inline static vk::DescriptorSet myDescriptorSet;

    inline static std::atomic_uint myNextArrayIndex = 0;
};
