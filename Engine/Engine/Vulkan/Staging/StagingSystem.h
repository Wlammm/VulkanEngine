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

    StagingBuffer GetStagingBufferWithSize(const uint inSize);

    void Tick();
    
private:
    void PrepareThisFramesStagingBuffer();
    
private:
    struct BufferData
    {
        ResizableBuffer* myBuffer;
        uint myOffset = 0;
    };

    uint myLastFrameIndex = (uint)-1;
    StagingSystem::BufferData* myCurrentStageData = nullptr;
    
    std::array<StagingSystem::BufferData, VulkanContext::FrameLag> myStagingBuffers;
};
