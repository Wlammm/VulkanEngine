#include "EnginePch.h"
#include "DebugPipeline.h"

#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Utils/Debug.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Assets/Material.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Transform.h"
#include "Vulkan/VulkanSwapChain.h"
#include "ECS/Systems/RenderSystem.h"
#include "Vulkan/VulkanImage.h"
#include "World/World.h"

DebugPipeline::DebugPipeline()
{
	myVertexShader = Engine::GetAssetRegistry().GetShader("Debug.vert");
	myVertexShader->AddObserver(this);

	myFragmentShader = Engine::GetAssetRegistry().GetShader("Debug.frag");
	myFragmentShader->AddObserver(this);

	myFrameDataBuffer = VulkanAllocator::AllocateBuffer_TS(
		"FrameDataBuffer",
		VulkanBuffer::UniformBufferCreateInfo(sizeof(FrameData)),
		VMA_MEMORY_USAGE_AUTO,
		true);

	CreateDescriptorSets();
	CreatePipeline();
	CreateRenderPass();
}

DebugPipeline::~DebugPipeline()
{
	VulkanAllocator::DestroyBuffer_TS(myFrameDataBuffer);
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
	VulkanContext::GetDevice()->destroyRenderPass(myRenderPass);
}

void DebugPipeline::AddDrawCommands(const vk::CommandBuffer inCommandBuffer)
{
	VulkanBuffer* vertexBuffer = BuildVertexBuffer();

	if (!vertexBuffer)
		return;

	RenderSystem* renderSystem = Engine::GetSystem<RenderSystem>();
	if (!renderSystem)
		return;

	UpdateFrameBuffer();
	
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});

	inCommandBuffer.bindVertexBuffers(0, { vertexBuffer->GetAPIResource() }, { 0 });
	inCommandBuffer.draw(Debug::GetDrawInfos().size() * 2, 1, 0, 0);
	
	Debug::ClearDrawInfos();
}

void DebugPipeline::OnAssetUpdated()
{

}

void DebugPipeline::CreateRenderPass()
{
	RenderSystem* renderSystem = Engine::GetSystem<RenderSystem>();

	check(renderSystem);
	const std::array<vk::AttachmentDescription, 2> attachments = {
		vk::AttachmentDescription()
			.setFormat(VulkanContext::GetSwapChain().GetFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
		vk::AttachmentDescription()
			.setFormat(renderSystem->GetDepthTexture()->GetFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal),
	};

	const auto colorReference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
	const auto depthReference = vk::AttachmentReference().setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	const auto subpass = vk::SubpassDescription()
		.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		.setColorAttachments(colorReference)
		.setPDepthStencilAttachment(&depthReference);

	vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	const std::array<vk::SubpassDependency, 2> dependencies = {
		vk::SubpassDependency()  // Depth buffer is shared between swapchain images
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(stages)
			.setDstStageMask(stages)
			.setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDependencyFlags(vk::DependencyFlags()),
		vk::SubpassDependency()
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask(vk::AccessFlagBits())
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead)
			.setDependencyFlags(vk::DependencyFlags())
	};

	myRenderPass = VulkanContext::GetDevice()->createRenderPass(vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
}

void DebugPipeline::CreatePipeline()
{
	List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSet.GetLayout() };
	myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));

	const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(*myVertexShader).setPName("main"),
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(*myFragmentShader).setPName("main"),
	};
	
	List<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions{};
	vertexInputAttributeDescriptions.Add({ 0, 0, vk::Format::eR32G32B32Sfloat, 0 });

	List<vk::VertexInputBindingDescription> bindingDescriptions;
	bindingDescriptions.Add(vk::VertexInputBindingDescription()
				.setBinding(0)
				.setStride(sizeof(glm::vec3))
				.setInputRate(vk::VertexInputRate::eVertex)
			);
	
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo().setVertexAttributeDescriptions(vertexInputAttributeDescriptions).setVertexBindingDescriptions(bindingDescriptions);

	const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(vk::PrimitiveTopology::eLineList);
	const vk::PipelineViewportStateCreateInfo viewportInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(1);
	const vk::PipelineRasterizationStateCreateInfo rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
		.setDepthClampEnable(VK_FALSE)
		.setRasterizerDiscardEnable(VK_FALSE)
		.setPolygonMode(vk::PolygonMode::eFill)
		.setCullMode(vk::CullModeFlagBits::eBack)
		.setFrontFace(vk::FrontFace::eCounterClockwise)
		.setDepthBiasEnable(VK_FALSE)
		.setLineWidth(1.0f);

	const auto multisampleInfo = vk::PipelineMultisampleStateCreateInfo();
	const auto stencilOp = vk::StencilOpState().setFailOp(vk::StencilOp::eKeep).setPassOp(vk::StencilOp::eKeep).setCompareOp(vk::CompareOp::eAlways);

	const auto depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo()
		.setDepthTestEnable(VK_TRUE)
		.setDepthWriteEnable(VK_TRUE)
		.setDepthCompareOp(vk::CompareOp::eLessOrEqual)
		.setDepthBoundsTestEnable(VK_FALSE)
		.setStencilTestEnable(VK_FALSE)
		.setFront(stencilOp)
		.setBack(stencilOp);

	const std::array<vk::PipelineColorBlendAttachmentState, 1> colorBlendAttachments = {
		vk::PipelineColorBlendAttachmentState().setColorWriteMask(
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA) };

	const vk::PipelineColorBlendStateCreateInfo colorBlendInfo = vk::PipelineColorBlendStateCreateInfo().setAttachments(colorBlendAttachments);
	const std::array<vk::DynamicState, 2> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
	const vk::PipelineDynamicStateCreateInfo dynamicStateInfo = vk::PipelineDynamicStateCreateInfo().setDynamicStates(dynamicStates);

	const vk::ResultValue<vk::Pipeline> returnValue = VulkanContext::GetDevice()->createGraphicsPipeline(VulkanContext::GetPipelineCache(), vk::GraphicsPipelineCreateInfo()
																										 .setStages(shaderStageInfo)
																										 .setPVertexInputState(&vertexInputInfo)
																										 .setPInputAssemblyState(&inputAssemblyInfo)
																										 .setPViewportState(&viewportInfo)
																										 .setPRasterizationState(&rasterizationInfo)
																										 .setPMultisampleState(&multisampleInfo)
																										 .setPDepthStencilState(&depthStencilInfo)
																										 .setPColorBlendState(&colorBlendInfo)
																										 .setPDynamicState(&dynamicStateInfo)
																										 .setLayout(myPipelineLayout)
																										 .setRenderPass(Engine::GetSystem<RenderSystem>()->GetRenderPass()));

	check(returnValue.result == vk::Result::eSuccess);
	myPipeline = returnValue.value;
}

void DebugPipeline::CreateDescriptorSets()
{
	myFrameDescriptorSet.BindBuffer(myFrameDataBuffer, vk::ShaderStageFlagBits::eVertex, 0, vk::DescriptorType::eUniformBuffer);
	myFrameDescriptorSet.Build();
}

VulkanBuffer* DebugPipeline::BuildVertexBuffer()
{
	List<glm::vec3> vertices{};

	if (Debug::GetDrawInfos().IsEmpty())
		return nullptr;

	for(const Debug::DrawLineInfos& lineInfos : Debug::GetDrawInfos())
	{
		vertices.Add(lineInfos.myStart);
		vertices.Add(lineInfos.myEnd);
	}

	vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
		.setSize(vertices.size() * sizeof(glm::vec3))
		.setUsage(vk::BufferUsageFlagBits::eVertexBuffer);

	VulkanBuffer* buffer = VulkanAllocator::AllocateBuffer_TS("DebugLines VertexBuffer", createInfo, VMA_MEMORY_USAGE_AUTO, true);
	buffer->SetData(vertices.data(), vertices.size() * sizeof(glm::vec3));
	
	VulkanAllocator::DestroyBuffer_TS(buffer);
	return buffer;
}

void DebugPipeline::UpdateFrameBuffer()
{
	auto view = Engine::GetWorld().GetRegistry().view<const Transform, const Camera>();

	for (auto ent : view)
	{
		FrameData data{};
		data.myProjection = view.get<const Camera>(ent).myProjection;
		data.myToView = glm::affineInverse(view.get<const Transform>(ent).GetMatrix());
		data.myCameraPosition = view.get<const Transform>(ent).GetPosition();
		myFrameDataBuffer->SetData(data);
		return;
	}

	LOG("No render camera found.");
}
