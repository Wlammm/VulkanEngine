#pragma once
#include "Subsystem/System.h"

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
    inline static MutexList<Texture*> myTexturesToRegister{};

    List<Texture*> myTextures;
    
    vk::DescriptorPool myDescriptorPool;
    
    vk::DescriptorSetLayout myDescriptorLayout;
    vk::DescriptorSet myDescriptorSet;

    uint myNextArrayIndex = 0;
};
