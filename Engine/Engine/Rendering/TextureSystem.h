#pragma once
#include "Engine/System/System.h"

class TextureCube;
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
    static void RegisterTextureCube_TS(TextureCube* inTexture);

    static void RemoveTexture_TS(Texture* inTexture);
    static void RemoveTextureCube_TS(TextureCube* inTexture);
    
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
    inline static MutexList<uint> myFreeIndices;
    
    inline static MutexList<TextureCube*> myTextureCubes;
    inline static MutexList<uint> myFreeCubeIndices;

    inline static SharedPtr<Texture> myMissingMaterialTexture = nullptr;
    
    vk::DescriptorPool myDescriptorPool;
    
    inline static vk::DescriptorSetLayout myDescriptorLayout;
    inline static vk::DescriptorSet myDescriptorSet;

    inline static std::atomic_uint myNextArrayIndex = 0;
};
