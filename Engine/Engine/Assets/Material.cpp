#include "EnginePch.h"
#include "Material.h"
#include "Engine.h"
#include "ECS/Systems/RenderSystem.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Vulkan/VulkanPipeline.h"
#include "Vulkan/VulkanTexture.h"
#include "Vulkan/VulkanShader.h"

Material::Material()
{
	myTexture = new VulkanTexture("Assets/Leaves.tga", SamplerMode::Wrap);
	myVertexShader = Engine::GetAssetRegistry().GetShader("VertexShader.vert");
	myPixelShader = Engine::GetAssetRegistry().GetShader("FragmentShader.frag");
	Create();

	myVertexShader->AddObserver(this);
	myPixelShader->AddObserver(this);
}

Material::~Material()
{
	myVertexShader->RemoveObserver(this);
	myPixelShader->RemoveObserver(this);
	Destroy();
	del(myTexture);
}

void Material::Create()
{
	RenderSystem* renderSystem = Engine::GetSystem<RenderSystem>();
	if (!renderSystem)
		check(false);

	VulkanPipeline::CreateInfo createInfo;
	createInfo.VertexShader = myVertexShader;
	createInfo.FragmentShader = myPixelShader;
	createInfo.RenderPass = renderSystem->GetRenderPass();
	createInfo.UniformBuffers = { &renderSystem->GetFrameBuffer(), &renderSystem->GetObjectBuffer()};
	createInfo.Textures = { myTexture };

	myPipeline = new VulkanPipeline(createInfo);
}

void Material::Destroy()
{
	del(myPipeline);
}

void Material::Bind(vk::CommandBuffer inCommandBuffer) const
{
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline->GetPipeline());
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipeline->GetPipelineLayout(), 0, myPipeline->GetDescriptorSet(), {});
}

void Material::OnAssetUpdated()
{
	Destroy();
	Create();
}
