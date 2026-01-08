#pragma once
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/System/System.h"
#include "Engine/Vulkan/Containers/IGPUBuffer.hpp"

class GPUResourceManager : public System
{
    inline static GPUResourceManager* myInstance = nullptr;
public:
    static GPUResourceManager* Get() { return myInstance; }
    
    GPUResourceManager();
    ~GPUResourceManager();
    
    struct BufferResource
    {
        IGPUBuffer* myBuffer;
        const Type* myType = nullptr;
        Delegate<void(BufferResource& inResource)> myTickFunction;
    };
    
    void Tick();
    
    /*
     * The resource system takes ownership of the buffer and its lifetime from here on.
     */
    template <typename T>
    void RegisterBuffer(IGPUBuffer* inBuffer, Delegate<void(BufferResource& inResource)> inTickFunction = nullptr)
    {
        const Type* type = ReflectionSystem::GetType<T>();

        for (BufferResource& buffer : myBuffers)
        {
            check(buffer.myType != type);
        }
        
        BufferResource& buffer = myBuffers.Emplace();
        buffer.myBuffer = inBuffer;
        buffer.myType = type;
        buffer.myTickFunction = inTickFunction;
        
        if (inTickFunction.IsValid())
        {
            myTickableBuffers.Add(buffer);
        }
    }

    template <typename T>
    IGPUBuffer* GetBuffer() const
    {
        const Type* type = ReflectionSystem::GetType<T>();
        return GetBuffer(type);
    }

    IGPUBuffer* GetBuffer(const std::string& inBufferTypeName) const;
    
    IGPUBuffer* GetBuffer(const Type* inType) const;
    
private:
    List<BufferResource> myBuffers;
    List<BufferResource> myTickableBuffers;
};
