#pragma once
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanImage.h"
#include "ECS/Systems/RenderSystem.h"
#include "Rendering/ShadowPipeline.h"

struct DirectionalLight
{
	Color myColor;
	Mat4f myLightView;
	Mat4f myLightProjection;

	// These are temporarily mutable so we can have createshadowmap const until we work with bindless textures for shadow maps.
	mutable VulkanImage* myShadowMap;
	mutable vk::Framebuffer myFrameBuffer;

	~DirectionalLight()
	{
		if(myShadowMap)
			VulkanAllocator::DestroyImage_TS(myShadowMap);

		if (myFrameBuffer)
			VulkanContext::GetDevice()->destroyFramebuffer(myFrameBuffer);
	}

	void CreateShadowMap(const Vec2ui& inSize) const 
	{
		myShadowMap = VulkanAllocator::AllocateImage_TS("DirectionalLight ShadowMap", VulkanImage::ShadowMapCreateInfo(inSize), VMA_MEMORY_USAGE_AUTO);
		myShadowMap->CreateDepthView();
		
		std::array<vk::ImageView, 1> attachments = { myShadowMap->GetImageView() };
		myFrameBuffer = VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
									.setRenderPass(RenderSystem::GetShadowPipeline().GetRenderPass())
									.setAttachments(attachments)
									.setWidth(myShadowMap->GetSize().x)
									.setHeight(myShadowMap->GetSize().y)
									.setLayers(1));
	}
};