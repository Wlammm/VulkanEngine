#pragma once
#include "StagingBuffer.h"
#include "Engine/System/System.h"
#include "Engine/Vulkan/VulkanContext.h"


class ResizableBuffer;

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
        ResizableBuffer* myBuffer;
        uint myOffset;
    };

    inline static std::mutex myMutex{};
    
    inline static uint myLastFrameIndex = (uint)-1;
    inline static StagingSystem::BufferData* myCurrentStageData = nullptr;
    
    inline static std::array<StagingSystem::BufferData, VulkanContext::FrameLag> myStagingBuffers;
};
