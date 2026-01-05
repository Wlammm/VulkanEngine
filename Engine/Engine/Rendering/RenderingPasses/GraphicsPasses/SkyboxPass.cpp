#include "EnginePch.h"
#include "SkyboxPass.h"

#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/Model.h"
#include "Engine/Assets/Texture.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Rendering/IndexBufferSystem.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/VertexBufferSystem.h"
#include "Engine/World/World.h"

SkyboxPass::SkyboxPass()
    : GraphicsPass("Shaders/SkyboxVS.hlsl", "Shaders/SkyboxPS.hlsl")
{
    mySkybox = Engine::GetEngineSystem<AssetRegistry>().GetAssetSynchronous<Texture>("Assets/Cubemaps/sunflowers_puresky_4k.hdr");
    mySkyboxModel = Engine::GetEngineSystem<AssetRegistry>().GetAssetSynchronous<Model>("Assets/Primitives/Sphere.fbx");
}

SkyboxPass::~SkyboxPass()
{
}

void SkyboxPass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eAttachmentOptimal, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore);
}

void SkyboxPass::SetupDescriptors()
{
    myDescriptorSet.BindBuffer(
        GPUResourceManager::Get()->GetBuffer<CameraBuffer>(),
        vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 
        0, 
        vk::DescriptorType::eUniformBuffer);
    
    myDescriptorSet.BindSampler(VulkanUtils::GetSampler(SamplerMode::LinearWrap), vk::ShaderStageFlagBits::eFragment, 2);
    myDescriptorSet.Build();
}

void SkyboxPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    if (!mySkyboxModel)
        return;

    inCommandBuffer.setDepthWriteEnable(false);
    // Invert the sphere for this pass as the camera is inside of it.
    inCommandBuffer.setFrontFace(vk::FrontFace::eClockwise);
    
    Mesh* mesh = mySkyboxModel->GetMeshes()[0];
    
    VertexBufferSystem& vertexBufferSystem = Engine::GetEngineSystem<VertexBufferSystem>();
    IndexBufferSystem& indexBufferSystem = Engine::GetEngineSystem<IndexBufferSystem>();
	
    uint vertexOffset = vertexBufferSystem.GetVertexOffsetFromVertexHandle(mesh->GetVertexBuffer());
    const IndexBufferData& indexData = indexBufferSystem.GetIndexBufferDataFromIndexHandle(mesh->GetIndexBuffer());
    inCommandBuffer.drawIndexed(indexData.myCount, 1, indexData.myOffset, vertexOffset, 0);
}
