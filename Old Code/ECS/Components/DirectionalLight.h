#pragma once
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanImage.h"
#include "ECS/Systems/RenderSystem.h"
#include "Rendering/ShadowPipeline.h"

struct DirectionalLight
{
	glm::vec4 myColor;
	glm::mat4 myLightView;
	glm::mat4 myLightProjection;

	// These are temporarily mutable so we can have createshadowmap const until we work with bindless textures for shadow maps.
	mutable VulkanImage* myShadowMap;
	mutable vk::Framebuffer myFrameBuffer;

	~DirectionalLight()
	{
		if(myShadowMap)
			VulkanAllocator::DestroyImage_TS(myShadowMap);

		LOG_WARNING("~DirectionalLight waiting for device idle.");
		VulkanContext::GetDevice()->waitIdle();
		if (myFrameBuffer)
			VulkanContext::GetDevice()->destroyFramebuffer(myFrameBuffer);
	}

	void CreateShadowMap(const glm::vec2& inSize) const 
	{
		myShadowMap = VulkanAllocator::AllocateImage_TS("DirectionalLight ShadowMap", VulkanImage::ShadowMapCreateInfo(inSize), VMA_MEMORY_USAGE_AUTO);
		myShadowMap->CreateDepthView();
		
		std::array<vk::ImageView, 1> attachments = { myShadowMap->GetImageView() };
		myFrameBuffer = VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
									.setRenderPass(RenderSystem::GetShadowPipeline().GetRenderPass())
									.setAttachments(attachments)
									.setWidth(static_cast<uint>(myShadowMap->GetSize().x))
									.setHeight(static_cast<uint>(myShadowMap->GetSize().y))
									.setLayers(1));
	}
};