#pragma once
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/System/System.h"
#include "Engine/Vulkan/Containers/IGPUBuffer.hpp"

class VulkanBuffer;

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
        List<std::string> myShaderAliases;
        bool myIsOwned = true;
        Delegate<void(BufferResource& inResource)> myTickFunction;
    };

    struct AccelerationStructureResource
    {
        vk::AccelerationStructureKHR myAccelerationStructure{};
        VulkanBuffer* myBuffer = nullptr;
        List<std::string> myShaderAliases;
        bool myIsOwned = false;
    };

    void Tick();
    
    /*
     * The resource system takes ownership of the buffer and its lifetime from here on.
     */
    template <typename T>
    void RegisterBuffer(IGPUBuffer* inBuffer, Delegate<void(BufferResource& inResource)> inTickFunction = nullptr)
    {
        RegisterBuffer<T>(inBuffer, {}, inTickFunction);
    }

    template <typename T>
    void RegisterBuffer(IGPUBuffer* inBuffer, List<std::string> inShaderAliases, Delegate<void(BufferResource& inResource)> inTickFunction = nullptr)
    {
        const Type* type = ReflectionSystem::GetType<T>();

        for (BufferResource& buffer : myBuffers)
        {
            check(buffer.myType != type);
        }

        BufferResource& buffer = myBuffers.Emplace();
        buffer.myBuffer = inBuffer;
        buffer.myType = type;
        buffer.myShaderAliases = std::move(inShaderAliases);
        buffer.myTickFunction = inTickFunction;

        if (inTickFunction.IsValid())
        {
            myTickableBuffers.Add(buffer);
        }
    }

    // Register a buffer that is not reflected (e.g. primitive-typed or Vulkan-struct buffers).
    // Lifetime is managed by the caller — GPUResourceManager will NOT destroy it.
    void RegisterBuffer(IGPUBuffer* inBuffer, List<std::string> inShaderAliases);

    // Register an acceleration structure. When inIsOwned is true, the manager will destroy
    // the AS and its backing buffer on shutdown.
    void RegisterAccelerationStructure(vk::AccelerationStructureKHR inAS, VulkanBuffer* inBuffer,
                                       List<std::string> inShaderAliases, bool inIsOwned = false);

    vk::AccelerationStructureKHR GetAccelerationStructure(const std::string& inAlias) const;
    vk::AccelerationStructureKHR TryGetAccelerationStructure(const std::string& inAlias) const;

    template <typename T>
    IGPUBuffer* GetBuffer() const
    {
        const Type* type = ReflectionSystem::GetType<T>();
        return GetBuffer(type);
    }

    IGPUBuffer* GetBuffer(const std::string& inBufferTypeName) const;
    IGPUBuffer* TryGetBuffer(const std::string& inBufferTypeName) const;
    IGPUBuffer* TryGetBufferByAlias(const std::string& inAlias) const;

    IGPUBuffer* GetBuffer(const Type* inType) const;
    
private:
    List<BufferResource> myBuffers;
    List<BufferResource> myTickableBuffers;
    List<AccelerationStructureResource> myAccelerationStructures;
};
