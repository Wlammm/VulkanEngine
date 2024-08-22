#include "EnginePch.h"
#include "DebugPipeline.h"

#include "Engine.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Shader.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Utils/Debug.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"

DebugPipeline::DebugPipeline()
{
	myVertexShader = Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("Debug.vert");
	myVertexShader->OnShaderRecompiled.Bind(&DebugPipeline::OnShaderRecompiled, this);

	myFragmentShader = Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("Debug.frag");
	myFragmentShader->OnShaderRecompiled.Bind(&DebugPipeline::OnShaderRecompiled, this);

	myFrameDataBuffer = VulkanAllocator::AllocateBuffer_TS(
		"FrameDataBuffer",
		VulkanBuffer::UniformBufferCreateInfo(sizeof(FrameData)),
		VMA_MEMORY_USAGE_AUTO,
		true);

	CreateDescriptorSets();
	CreatePipeline();
}

DebugPipeline::~DebugPipeline()
{
	VulkanAllocator::DestroyBuffer_TS(myFrameDataBuffer);
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	myVertexShader->OnShaderRecompiled.UnBind(&DebugPipeline::OnShaderRecompiled, this);
	myFragmentShader->OnShaderRecompiled.UnBind(&DebugPipeline::OnShaderRecompiled, this);
}

void DebugPipeline::AddDrawCommands(const vk::CommandBuffer inCommandBuffer)
{
	VulkanBuffer* vertexBuffer = BuildVertexBuffer();

	if (!vertexBuffer)
		return;

	UpdateFrameBuffer();
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});

	inCommandBuffer.bindVertexBuffers(0, { vertexBuffer->GetAPIResource() }, { 0 });
	inCommandBuffer.draw(Debug::GetDrawInfos().size() * 2, 1, 0, 0);
	
	Debug::ClearDrawInfos();
}

void DebugPipeline::OnShaderRecompiled()
{
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	CreatePipeline();
}

void DebugPipeline::CreatePipeline()
{
	List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSet.GetLayout() };
	myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));

	const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(myVertexShader->GetAPIResource()).setPName("main"),
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(myFragmentShader->GetAPIResource()).setPName("main"),
	};
	
	List<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions{};
	vertexInputAttributeDescriptions.Add({ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(DebugVertex, myPosition) });
	vertexInputAttributeDescriptions.Add({ 1, 0, vk::Format::eR32Sint, offsetof(DebugVertex, myColor) });

	List<vk::VertexInputBindingDescription> bindingDescriptions;
	bindingDescriptions.Add(vk::VertexInputBindingDescription()
				.setBinding(0)
				.setStride(sizeof(DebugVertex))
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
																										 .setRenderPass(Engine::GetEngineSystem<RenderSystem>().GetRenderPass()));
 
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
	List<DebugVertex> vertices{};

	if (Debug::GetDrawInfos().IsEmpty())
		return nullptr;

	for(const Debug::DrawLineInfos& lineInfos : Debug::GetDrawInfos())
	{
		vertices.Add({lineInfos.myStart, lineInfos.myColor });
		vertices.Add({lineInfos.myEnd, lineInfos.myColor });
	}

	vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
		.setSize(vertices.size() * sizeof(DebugVertex))
		.setUsage(vk::BufferUsageFlagBits::eVertexBuffer);

	VulkanBuffer* buffer = VulkanAllocator::AllocateBuffer_TS("DebugLines VertexBuffer", createInfo, VMA_MEMORY_USAGE_AUTO, true);
	buffer->SetData(vertices.data(), vertices.size() * sizeof(DebugVertex));
	
	VulkanAllocator::DestroyBuffer_TS(buffer);
	return buffer;
}

void DebugPipeline::UpdateFrameBuffer()
{
	LOG("DebugPipeline::UpdateFrameBuffer commented out.");
	//for (auto [ent, transform, camera] : Engine::GetWorld().GetRegistry().IterateComponents<const Transform, const Camera>())
	//{
	//	FrameData data{};
	//	data.myProjection = camera.myProjection;
	//	data.myToView = glm::affineInverse(transform.GetMatrix());
	//	data.myCameraPosition = transform.GetPosition();
	//	myFrameDataBuffer->SetData(data);
	//	return;
	//}

	LOG("No render camera found.");
}
