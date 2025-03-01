#include "EnginePch.h"
#include "SkyboxPipeline.h"

#include "Engine.h"
#include "IndexBufferSystem.h"
#include "Mesh.h"
#include "TextureSystem.h"
#include "VertexBufferHandle.h"
#include "VertexBufferSystem.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Model.h"
#include "Assets/Shader.h"
#include "Assets/Texture.h"
#include "Assets/TextureCube.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "World/World.h"

SkyboxPipeline::SkyboxPipeline()
{
	Engine::GetAssetRegistry().GetAssetAsync<Texture>("Assets/Cubemaps/sunflowers_puresky_4k.hdr", [this](Texture* inCubemap)
	{
		mySkybox = inCubemap;
	});

	Engine::GetAssetRegistry().GetAssetAsync<Model>("Assets/Primitives/Sphere.fbx", [this](Model* inModel)
	{
		mySkyboxModel = inModel;
	});
	
	myFrameDataBuffer = VulkanAllocator::AllocateBuffer_TS(
		"FrameDataBuffer",
		VulkanBuffer::UniformBufferCreateInfo(sizeof(FrameData)),
		VMA_MEMORY_USAGE_AUTO, 
		true);
    CreateDrawResources();
	CreateGraphicsPipeline();
}

SkyboxPipeline::~SkyboxPipeline()
{
	myVertexShader->OnShaderRecompiled.UnBind(&SkyboxPipeline::OnShaderRecompiled, this);
	myFragmentShader->OnShaderRecompiled.UnBind(&SkyboxPipeline::OnShaderRecompiled, this);
	
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	
	VulkanAllocator::DestroyBuffer_TS(myFrameDataBuffer);
}

void SkyboxPipeline::AddGraphicsCommands(vk::CommandBuffer inCommandBuffer)
{
	GPUMARK_SCOPE(inCommandBuffer, "Skybox");
	if(!mySkybox || !mySkyboxModel)
		return;
	
	VertexBufferSystem& vertexBufferSystem = Engine::GetEngineSystem<VertexBufferSystem>();
	IndexBufferSystem& indexBufferSystem = Engine::GetEngineSystem<IndexBufferSystem>();
	
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);

	BuildFrameBuffer();

	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});
	
	inCommandBuffer.bindVertexBuffers(0, {vertexBufferSystem.GetGlobalVertexBuffer()->GetBuffer()->GetAPIResource()}, {0});
	
	inCommandBuffer.bindIndexBuffer(indexBufferSystem.GetGlobalIndexBuffer()->GetBuffer()->GetAPIResource(),0, vk::IndexType::eUint32);
	
	TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 1, textureSystem.GetDescriptorSet(), {});

	Mesh* mesh = mySkyboxModel->GetMeshes()[0];
	
	uint vertexOffset = vertexBufferSystem.GetVertexOffsetFromVertexHandle(mesh->GetVertexBuffer());
	const IndexBufferData& indexData = indexBufferSystem.GetIndexBufferDataFromIndexHandle(mesh->GetIndexBuffer());
	
	inCommandBuffer.drawIndexed(indexData.myCount, 1, indexData.myOffset, vertexOffset, 0);
}

void SkyboxPipeline::OnShaderRecompiled()
{
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	CreateGraphicsPipeline();
}

void SkyboxPipeline::CreateDrawResources()
{
    myVertexShader = Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("Skybox.vert");
    myVertexShader->OnShaderRecompiled.Bind(&SkyboxPipeline::OnShaderRecompiled, this);
    
    myFragmentShader = Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("Skybox.frag");
    myFragmentShader->OnShaderRecompiled.Bind(&SkyboxPipeline::OnShaderRecompiled, this);
    
    // Descriptor sets
    myFrameDescriptorSet.BindBuffer(
        myFrameDataBuffer, 
        vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 
        0, 
        vk::DescriptorType::eUniformBuffer);
    
    myFrameDescriptorSet.Build();
}

void SkyboxPipeline::CreateGraphicsPipeline()
{
    TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
	
	const List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSet.GetLayout(), textureSystem.GetDescriptorLayout() };
	myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));

	const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(myVertexShader->GetAPIResource()).setPName("main"),
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(myFragmentShader->GetAPIResource()).setPName("main"),
	};

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo().setVertexAttributeDescriptions(Vertex::GetAttributeDescriptions()).setVertexBindingDescriptions(Vertex::GetBindingDescriptions());

	const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(vk::PrimitiveTopology::eTriangleList);
	const vk::PipelineViewportStateCreateInfo viewportInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(1);
	const vk::PipelineRasterizationStateCreateInfo rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
		.setDepthClampEnable(VK_FALSE)
		.setRasterizerDiscardEnable(VK_FALSE)
		.setPolygonMode(vk::PolygonMode::eFill)
		.setCullMode(vk::CullModeFlagBits::eBack)
		.setFrontFace(vk::FrontFace::eCounterClockwise)
		.setDepthBiasEnable(VK_FALSE)
		.setLineWidth(1.0f);

	const auto multisampleInfo = vk::PipelineMultisampleStateCreateInfo().setRasterizationSamples(VulkanContext::GetPhysicalDevice().GetMaxMSAASamples());
	const auto stencilOp = vk::StencilOpState().setFailOp(vk::StencilOp::eKeep).setPassOp(vk::StencilOp::eKeep).setCompareOp(vk::CompareOp::eAlways);

	const auto depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo()
		.setDepthTestEnable(VK_TRUE)
		.setDepthWriteEnable(VK_FALSE)
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

void SkyboxPipeline::BuildFrameBuffer()
{
	if (!mySkybox)
		return;

	CameraComponent* camera = Engine::GetWorld().GetComponentSystem().GetAnyComponentOfType<CameraComponent>();
	if(!camera)
		return;
		
	FrameData data{};
	data.myProjection = camera->GetProjection();
	data.myToView = glm::affineInverse(camera->GetTransform()->GetMatrix());
	data.myCameraPosition = camera->GetTransform()->GetPosition();
	data.myCubemapIndex = mySkybox->GetBindlessIndex();
	
	myFrameDataBuffer->SetData(data);
}
