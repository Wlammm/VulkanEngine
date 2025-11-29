#pragma once
#include "Engine/Assets/Texture.h"
#include "Engine/Vulkan/VulkanDescriptorSet.h"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Vulkan/VulkanUtils.hpp"

class ImGuiTextureUtils
{
public:
    static vk::DescriptorSet CreateDescriptorSetForTexture(SharedPtr<Texture> inTexture)
    {
        if (!inTexture->GetImage()->HasImageView())
            inTexture->GetImage()->CreateView(vk::ImageViewType::e2D);
        
        vk::DescriptorSet descriptorSet = ImGui_ImplVulkan_AddTexture(VulkanUtils::GetSampler(SamplerMode::Clamp), inTexture->GetImage()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        return descriptorSet;
    }
};
