#ifndef POINTLIGHTBUFFER_HPP
#define POINTLIGHTBUFFER_HPP
#include "../SharedHeader.hpp"
#include "../MeshStructs.hpp"

DYNAMIC_BUFFER(PointLightDataBuffer, PointLightData, inPointLightBuffer, ENGINE_DESCRIPTOR_SET_INDEX, 1, readonly buffer, VulkanBuffer::ResizableStorageBufferCreateInfo(sizeof(PointLightData) * 16), "PointLightBuffer", VMA_MEMORY_USAGE_AUTO, 16);

#endif // POINTLIGHTBUFFER_HPP
