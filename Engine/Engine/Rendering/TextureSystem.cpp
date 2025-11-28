#include "EnginePch.h"
#include "TextureSystem.h"

#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/AssetRegistry/AssetRegistry2.h"
#include "Engine/Assets/Texture.h"
#include "Engine/Assets/TextureCube.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanImage.h"

static const uint MAX_BINDLESS_RESOURCES = 16536;
static const uint BINDLESS_BINDING = 0;

TextureSystem::TextureSystem()
{
    CreateDescritorPool();
    CreateDescriptorSetLayout();
    CreateDescriptorSet();

    check(!myMissingMaterialTexture && "This variable is static so make sure we dont create multiple in case we ever create multiple instances of this system.");
    myMissingMaterialTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>("MissingTexture.png");
}

TextureSystem::~TextureSystem()
{
    myMissingMaterialTexture = nullptr;
    
    LOG_WARNING("TextureSystem::~TextureSystem is waiting for GPU idle!");
    VulkanContext::GetDevice()->waitIdle();
    VulkanContext::GetDevice()->destroyDescriptorSetLayout(myDescriptorLayout);
    VulkanContext::GetDevice()->destroyDescriptorPool(myDescriptorPool);
    myDescriptorSet = nullptr;
}

void TextureSystem::Tick()
{
    RegisterAllQueuedTextures();
}

void TextureSystem::RegisterTexture_TS(Texture* inTexture)
{
    vk::Sampler sampler = VulkanUtils::GetSampler(SamplerMode::Wrap);
    
    uint handle;
    myFreeIndices.Lock();
    if (myFreeIndices.size() != 0)
    {
        handle = myFreeIndices.Last();
        myFreeIndices.RemoveIndex(myFreeIndices.size() - 1);
    }
    else
    {
        handle = myNextArrayIndex++;
    }
    myFreeIndices.Unlock();
    
    const vk::DescriptorImageInfo imageInfo = vk::DescriptorImageInfo()
        .setSampler(sampler)
        .setImageView(inTexture->GetImageView())
        .setImageLayout(vk::ImageLayout::eReadOnlyOptimal);

    const vk::WriteDescriptorSet write = vk::WriteDescriptorSet()
        .setDescriptorCount(1)
        .setDstArrayElement(handle)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setDstSet(myDescriptorSet)
        .setDstBinding(BINDLESS_BINDING)
        .setImageInfo(imageInfo);

    VulkanContext::GetDevice()->updateDescriptorSets({write}, {});
    inTexture->myBindlessIndex = handle;
    myTextures.Add(inTexture);
    
    //myTexturesToRegister.Add(inTexture);
}

void TextureSystem::RegisterTextureCube_TS(TextureCube* inTexture)
{
    vk::Sampler sampler = VulkanUtils::GetSampler(SamplerMode::Wrap);
    uint handle = myNextArrayIndex++;
    const vk::DescriptorImageInfo imageInfo = vk::DescriptorImageInfo()
        .setSampler(sampler)
        .setImageView(inTexture->GetImageView())
        .setImageLayout(vk::ImageLayout::eReadOnlyOptimal);

    const vk::WriteDescriptorSet write = vk::WriteDescriptorSet()
        .setDescriptorCount(1)
        .setDstArrayElement(handle)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setDstSet(myDescriptorSet)
        .setDstBinding(BINDLESS_BINDING)
        .setImageInfo(imageInfo);

    VulkanContext::GetDevice()->updateDescriptorSets({write}, {});
    inTexture->myBindlessIndex = handle;
    myTextureCubes.Add(inTexture);
}

void TextureSystem::RemoveTexture_TS(Texture* inTexture)
{
    if (!myDescriptorSet)
        return;
    
    if (inTexture->myBindlessIndex == UINT32_MAX)
        return;

    if (myMissingMaterialTexture)
    {
        vk::DescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = vk::ImageLayout::eReadOnlyOptimal;
        imageInfo.imageView = myMissingMaterialTexture->GetImageView();
        imageInfo.sampler = VulkanUtils::GetSampler(SamplerMode::Wrap);

        const vk::WriteDescriptorSet write = vk::WriteDescriptorSet()
            .setDescriptorCount(1)
            .setDstArrayElement(inTexture->myBindlessIndex)
            .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
            .setDstSet(myDescriptorSet)
            .setDstBinding(BINDLESS_BINDING)
            .setImageInfo(imageInfo);

        VulkanContext::GetDevice()->updateDescriptorSets({write}, {});
    }
    
    myFreeIndices.Add(inTexture->myBindlessIndex);
    inTexture->myBindlessIndex = UINT32_MAX;

    myTextures.Remove(inTexture);
}

void TextureSystem::RemoveTextureCube_TS(TextureCube* inTexture)
{
    if (inTexture->myBindlessIndex == UINT32_MAX)
        return; 

    vk::DescriptorImageInfo nullImageInfo = {};
    nullImageInfo.imageLayout = vk::ImageLayout::eReadOnlyOptimal;
    nullImageInfo.imageView = myMissingMaterialTexture->GetImageView();
    nullImageInfo.sampler = VulkanUtils::GetSampler(SamplerMode::Wrap);

    const vk::WriteDescriptorSet write = vk::WriteDescriptorSet()
        .setDescriptorCount(1)
        .setDstArrayElement(inTexture->myBindlessIndex)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setDstSet(myDescriptorSet)
        .setDstBinding(BINDLESS_BINDING)
        .setImageInfo(nullImageInfo);

    VulkanContext::GetDevice()->updateDescriptorSets({write}, {});

    myFreeCubeIndices.Add(inTexture->myBindlessIndex);
    inTexture->myBindlessIndex = UINT32_MAX;
    
    myTextureCubes.Remove(inTexture);
}

vk::DescriptorSet TextureSystem::GetDescriptorSet() const
{
    return myDescriptorSet;
}

vk::DescriptorSetLayout TextureSystem::GetDescriptorLayout() const
{
    return myDescriptorLayout;
}

void TextureSystem::RegisterAllQueuedTextures()
{
    //myTexturesToRegister.Lock();
//
//    for(Texture* texture : myTexturesToRegister)
//    {
//        RegisterTexture(texture);
//    }
//    myTexturesToRegister.Clear();
//    
//    myTexturesToRegister.Unlock();
}

void TextureSystem::RegisterTexture(Texture* inTexture)
{
    vk::Sampler sampler = VulkanUtils::GetSampler(SamplerMode::Clamp);
    uint handle = ++myNextArrayIndex;
    const vk::DescriptorImageInfo imageInfo = vk::DescriptorImageInfo()
        .setSampler(sampler)
        .setImageView(inTexture->GetImageView())
        .setImageLayout(vk::ImageLayout::eReadOnlyOptimal);

    const vk::WriteDescriptorSet write = vk::WriteDescriptorSet()
        .setDescriptorCount(1)
        .setDstArrayElement(handle)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setDstSet(myDescriptorSet)
        .setDstBinding(BINDLESS_BINDING)
        .setImageInfo(imageInfo);

    VulkanContext::GetDevice()->updateDescriptorSets({write}, {});
    inTexture->myBindlessIndex = handle;
    myTextures.Add(inTexture);
}

void TextureSystem::CreateDescritorPool()
{
    const List<vk::DescriptorPoolSize> sizes =
    {
    { vk::DescriptorType::eCombinedImageSampler, MAX_BINDLESS_RESOURCES }
    };

    vk::DescriptorPoolCreateInfo createInfo = vk::DescriptorPoolCreateInfo()
    .setFlags(vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind)
    .setMaxSets(MAX_BINDLESS_RESOURCES * sizes.size())
    .setPoolSizes(sizes);

    myDescriptorPool = VulkanContext::GetDevice()->createDescriptorPool(createInfo);
    
}

void TextureSystem::CreateDescriptorSetLayout()
{
    vk::DescriptorBindingFlags flags =
        vk::DescriptorBindingFlagBits::ePartiallyBound |
        vk::DescriptorBindingFlagBits::eVariableDescriptorCount |
        vk::DescriptorBindingFlagBits::eUpdateAfterBind;

    vk::DescriptorSetLayoutBinding binding;
    binding.setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
    binding.setDescriptorCount(MAX_BINDLESS_RESOURCES);
    binding.binding = BINDLESS_BINDING;

    binding.setStageFlags(vk::ShaderStageFlagBits::eAll);

    vk::DescriptorSetLayoutCreateInfo createInfo = vk::DescriptorSetLayoutCreateInfo()
        .setBindings({binding})
        .setFlags(vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool);

    vk::DescriptorSetLayoutBindingFlagsCreateInfo extendedInfo = vk::DescriptorSetLayoutBindingFlagsCreateInfo()
        .setBindingCount(1)
        .setBindingFlags(flags);

    createInfo.setPNext(&extendedInfo);

    myDescriptorLayout = VulkanContext::GetDevice()->createDescriptorSetLayout(createInfo);
}

void TextureSystem::CreateDescriptorSet()
{
    vk::DescriptorSetAllocateInfo allocInfo = vk::DescriptorSetAllocateInfo()
        .setDescriptorPool(myDescriptorPool)
        .setSetLayouts({myDescriptorLayout});

    const List<uint> bindingCounts = {MAX_BINDLESS_RESOURCES - 1};
    vk::DescriptorSetVariableDescriptorCountAllocateInfo countInfo = vk::DescriptorSetVariableDescriptorCountAllocateInfo()
        .setDescriptorCounts(bindingCounts);

    allocInfo.setPNext(&countInfo);

    myDescriptorSet = VulkanContext::GetDevice()->allocateDescriptorSets(allocInfo).front();
}
