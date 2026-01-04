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
    
    void Tick()
    {
        for (BufferResource& buffer : myBuffers)
        {
            buffer.myTickFunction(buffer);
        }
    }
    
    /*
     * The resource system takes ownership of the buffer and its lifetime from here on.
     */
    template <typename T>
    void RegisterBuffer(IGPUBuffer* inBuffer, Delegate<void(BufferResource& inResource)> inTickFunction = nullptr)
    {
        const Type* type = ReflectionSystem::GetType<T>();
        
        BufferResource& buffer = myBuffers.Emplace();
        buffer.myBuffer = inBuffer;
        buffer.myType = type;
        buffer.myTickFunction = inTickFunction;
        
        if (inTickFunction != nullptr)
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

    IGPUBuffer* GetBuffer(const std::string& inBufferTypeName) const
    {
        const Type* type = ReflectionSystem::GetTypeByName(inBufferTypeName);
        return GetBuffer(type);
    }
    
    IGPUBuffer* GetBuffer(const Type* inType) const
    {
        for (const BufferResource& buffer : myBuffers)
        {
            if (buffer.myType == inType)
                return buffer.myBuffer;
        }
        
        check(false && "Failed to find buffer for this type.");
        return nullptr;
    }
    
private:
    List<BufferResource> myBuffers;
    List<BufferResource> myTickableBuffers;
};
