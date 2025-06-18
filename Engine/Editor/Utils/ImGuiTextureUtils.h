#pragma once
#include "Editor/Editor.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/Texture.h"
#include "Engine/Vulkan/VulkanDescriptorSet.h"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Vulkan/VulkanUtils.hpp"

class ImGuiTextureUtils
{
public:
    static vk::DescriptorSet CreateDescriptorSetForTexture(const std::filesystem::path& inPath)
    {
        Texture* texture = Editor::GetAssetRegistry()->GetAssetSynchronous<Texture>(inPath);
        
        if (!texture->GetImage()->HasImageView())
            texture->GetImage()->CreateView(vk::ImageViewType::e2D);
        
        vk::DescriptorSet descriptorSet = ImGui_ImplVulkan_AddTexture(VulkanUtils::GetSampler(SamplerMode::Clamp), texture->GetImage()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        return descriptorSet;
    }
};
