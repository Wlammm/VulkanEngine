#pragma once
#include "Engine/Vulkan/Containers/IGPUAccelerationStructure.hpp"

class VulkanBuffer;

class BLAS : public IGPUAccelerationStructure
{
public:
	~BLAS();

	vk::AccelerationStructureKHR GetAccelerationStructure() const override { return myAccelerationStructure; }
	MulticastDelegate<void()>* GetOnRebuilt() const override { return &myOnRebuilt; }

	VkDeviceAddress GetDeviceAddress() const { return myDeviceAddress; }

	static BLAS* Build(const List<glm::vec3>& inPositions, const List<uint>& inIndices);

private:
	BLAS() = default;

	vk::AccelerationStructureKHR myAccelerationStructure{};
	VulkanBuffer* myBLASBuffer = nullptr;
	VkDeviceAddress myDeviceAddress = 0;

	mutable MulticastDelegate<void()> myOnRebuilt;
};