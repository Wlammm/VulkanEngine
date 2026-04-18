#pragma once
#include "Engine/Vulkan/Containers/IGPUAccelerationStructure.hpp"
#include "BLAS.h"

class VulkanBuffer;

class TLAS : public IGPUAccelerationStructure
{
public:
	~TLAS();

	// Rebuilds (or creates) the TLAS from the given set of instances. inBLASes must list
	// every BLAS referenced by inInstances so the render graph can insert the required
	// BLAS-write → TLAS-read memory barrier when both builds land in the same upload pass.
	void Build(const List<VkAccelerationStructureInstanceKHR>& inInstances, const List<BLAS*>& inBLASes);

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