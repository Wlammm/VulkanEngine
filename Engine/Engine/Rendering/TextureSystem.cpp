#include "EnginePch.h"
#include "TextureSystem.h"

#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDescriptorSet.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanImage.h"

static const uint MAX_BINDLESS_RESOURCES = 16536;
static const uint BINDLESS_BINDING = 0;

TextureSystem::TextureSystem()
{
    CreateDescritorPool();
    CreateDescriptorSetLayout();
    CreateDescriptorSet();   
}

TextureSystem::~TextureSystem()
{
    VulkanContext::GetDevice()->destroyDescriptorSetLayout(myDescriptorLayout);
    VulkanContext::GetDevice()->destroyDescriptorPool(myDescriptorPool);
    myDescriptorSet = nullptr;
}

TextureHandle TextureSystem::AddTexture(VulkanImage* inImage, const vk::Sampler inSampler)
{
    TextureHandle handle = ++myNextHandle;

    TextureData data{};
    data.myHandle = handle;
    data.myIndex = ++myNextArrayIndex;
    myTextures.insert({handle, data});

    const vk::DescriptorImageInfo imageInfo = vk::DescriptorImageInfo()
        .setSampler(inSampler)
        .setImageView(inImage->GetImageView())
        .setImageLayout(vk::ImageLayout::eReadOnlyOptimal);

    const vk::WriteDescriptorSet write = vk::WriteDescriptorSet()
        .setDescriptorCount(1)
        .setDstArrayElement(data.myIndex)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setDstSet(myDescriptorSet)
        .setDstBinding(BINDLESS_BINDING)
        .setImageInfo(imageInfo);

    VulkanContext::GetDevice()->updateDescriptorSets({write}, {});
    return handle;
}

vk::DescriptorSet TextureSystem::GetDescriptorSet() const
{
    return myDescriptorSet;
}

vk::DescriptorSetLayout TextureSystem::GetDescriptorLayout() const
{
    return myDescriptorLayout;
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
