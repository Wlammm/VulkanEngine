#pragma once

class VulkanRenderState
{
public:
	struct CreateInfo
	{
		std::filesystem::path VertexShader;
		std::filesystem::path FragmentShader;

		class VulkanRenderImages* ColorTargets = nullptr;
		class VulkanDepthBuffer* DepthBuffer = nullptr;

		List<class VulkanImage*> ImageResources;
		List<class VulkanUniformBuffer*> UniformBuffers;
	};

	VulkanRenderState() = delete;
	VulkanRenderState(const CreateInfo& inCreateInfo);

	void Begin();
	void End();

private:
	void CreateRenderPass(const CreateInfo& inCreateInfo);
	void CreateFramebuffer(const CreateInfo& inCreateInfo);

private:
	List<vk::Framebuffer> myFrameBuffers;
	vk::RenderPass myRenderPass;

};