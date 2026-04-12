#pragma once
#include "Engine/Vulkan/Containers/IGPUAccelerationStructure.hpp"

class VulkanBuffer;

class TLAS : public IGPUAccelerationStructure
{
public:
	~TLAS();

	// Rebuilds (or creates) the TLAS from the given set of instances. Persistent GPU
	// resources are reused across frames; they are only reallocated when the instance
	// count exceeds the previously allocated capacity.
	void Build(const List<VkAccelerationStructureInstanceKHR>& inInstances);

	vk::AccelerationStructureKHR GetAccelerationStructure() const override { return myAS; }
	MulticastDelegate<void()>* GetOnRebuilt() const override { return &myOnRebuilt; }

	VkDeviceAddress GetDeviceAddress() const { return myDeviceAddress; }
	bool IsBuilt() const { return static_cast<bool>(myAS); }

private:
	// Allocates (or reallocates) all GPU resources for the given instance count.
	void Recreate(uint inInstanceCount, const vk::AccelerationStructureBuildGeometryInfoKHR& inBuildInfo);

	// Deferred-safe cleanup of all held GPU resources.
	void DestroyResources();

	vk::AccelerationStructureKHR myAS{};
	VulkanBuffer* myASBuffer = nullptr;
	VulkanBuffer* myInstanceBuffer = nullptr;
	VulkanBuffer* myScratchBuffer = nullptr;

	uint myInstanceCapacity = 0;
	uint myScratchAlignment = 128;
	VkDeviceAddress myDeviceAddress = 0;

	mutable MulticastDelegate<void()> myOnRebuilt;
};