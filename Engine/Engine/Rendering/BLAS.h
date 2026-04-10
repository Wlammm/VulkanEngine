#pragma once

class VulkanBuffer;

class BLAS
{
public:
	~BLAS();

	vk::AccelerationStructureKHR GetAccelerationStructure() const { return myAccelerationStructure; }
	VkDeviceAddress GetDeviceAddress() const { return myDeviceAddress; }

	static BLAS* Build(const List<glm::vec3>& inPositions, const List<uint>& inIndices);

private:
	BLAS() = default;

	vk::AccelerationStructureKHR myAccelerationStructure{};
	VulkanBuffer* myBLASBuffer = nullptr;
	VkDeviceAddress myDeviceAddress = 0;
};