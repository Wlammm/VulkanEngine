#include "EnginePch.h"
#include "DebugPass.h"

#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Utils/Debug.h"
#include "Engine/World/World.h"

DebugPass::DebugPass()
    : GraphicsPass("Shaders/DebugVS.hlsl", "Shaders/DebugPS.hlsl", false, false)
{
    myFrameDataBuffer = VulkanAllocator::AllocateBuffer_TS(
        "FrameDataBuffer",
        VulkanBuffer::UniformBufferCreateInfo(sizeof(FrameData)),
        VMA_MEMORY_USAGE_AUTO,
        true);
}

DebugPass::~DebugPass()
{
    VulkanAllocator::DestroyBuffer_TS(myFrameDataBuffer);
}

void DebugPass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedRenderTexture());
    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore);
}

void DebugPass::SetupDescriptors()
{
    myDescriptorSet.BindBuffer(myFrameDataBuffer, vk::ShaderStageFlagBits::eVertex, 0, vk::DescriptorType::eUniformBuffer);
    myDescriptorSet.Build();
}

List<vk::VertexInputBindingDescription>& DebugPass::GetVertexBindingDescriptor() const
{
    static List<vk::VertexInputBindingDescription> bindingDescriptions;

    DO_ONCE(
        bindingDescriptions.Add(vk::VertexInputBindingDescription()
            .setBinding(0)
            .setStride(sizeof(DebugVertex))
            .setInputRate(vk::VertexInputRate::eVertex)
        );
    );

    return bindingDescriptions;
}

List<vk::VertexInputAttributeDescription>& DebugPass::GetVertexAttributeDescriptions() const
{
    static List<vk::VertexInputAttributeDescription> descs{};

    DO_ONCE(
        descs.Add({ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(DebugVertex, myPosition) });
        descs.Add({ 1, 0, vk::Format::eR32Sint, offsetof(DebugVertex, myColor) });
    );
    return descs;
}

vk::PrimitiveTopology DebugPass::GetPrimitiveTopology() const
{
    return vk::PrimitiveTopology::eLineList;
}

void DebugPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    VulkanBuffer* vertexBuffer = BuildVertexBuffer();
    
    if (!vertexBuffer)
        return;
    
    UpdateFrameBuffer();
    
    inCommandBuffer.bindVertexBuffers(0, { vertexBuffer->GetAPIResource() }, { 0 });
    inCommandBuffer.draw(Debug::GetDrawInfos().size() * 2, 1, 0, 0);
    Debug::ClearDrawInfos();
}

VulkanBuffer* DebugPass::BuildVertexBuffer()
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

void DebugPass::UpdateFrameBuffer()
{
    CameraComponent* camera = Engine::GetWorld()->GetMainCamera();

    if (!camera)
    {
        LOG_ERROR("No main camera set!");
        return;
    }
	
    TransformComponent& transform = camera->GetTransform();
	
    FrameData data{};
    data.myProjection = camera->GetProjection();
    data.myToView = glm::affineInverse(transform.GetMatrix());
    data.myCameraPosition = transform.GetPosition();
    myFrameDataBuffer->SetData(data);
}