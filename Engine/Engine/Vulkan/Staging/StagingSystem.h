#pragma once
#include "StagingBuffer.h"
#include "Engine/System/System.h"
#include "Engine/Vulkan/VulkanContext.h"


class VulkanBuffer;

class StagingSystem : public System
{
public:
    StagingSystem();
    ~StagingSystem();

    static StagingBuffer GetStagingBufferWithSize_TS(const uint inSize);

    void Tick();

private:
    void PrepareThisFramesStagingBuffer();

private:
    struct BufferData
    {
        VulkanBuffer* myBuffer = nullptr;
        uint myOffset = 0;
    };

    // One more slot than FrameLag: an upload staged after this frame's upload drain
    // (RenderSystem::AddUploadPass) is only recorded into the NEXT frame's command buffer, so the
    // GPU reads its staging memory one frame later than this slot's frame-in-flight fence
    // guarantees. Cycling FrameLag + 1 slots means a slot written during frame N is reused at
    // frame N + FrameLag + 1, by which point the CPU has waited on the fence of frame N + 1 — the
    // latest frame whose command buffer can reference that staging data.
    static constexpr uint NumStagingSlots = VulkanContext::FrameLag + 1;

    inline static std::mutex myMutex{};

    inline static uint myLastFrameIndex = (uint)-1;
    inline static StagingSystem::BufferData* myCurrentStageData = nullptr;

    inline static std::array<StagingSystem::BufferData, NumStagingSlots> myStagingBuffers;
};
