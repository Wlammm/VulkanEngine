#ifndef PERDRAWDATABUFFER_HPP
#define PERDRAWDATABUFFER_HPP
#include "../SharedHeader.hpp"
#include "../MeshStructs.hpp"
#include "Engine/Vulkan/VulkanBuffer.h"


DYNAMIC_BUFFER(PerDrawDataBuffer, PerDrawData, inPerDrawData, 0, 4, readonly buffer, VulkanBuffer::ResizableStorageBufferCreateInfo(sizeof(PointLightData) * 16), "PerDrawDataBuffer", VMA_MEMORY_USAGE_AUTO, 16);

#endif // PERDRAWDATABUFFER_HPP
