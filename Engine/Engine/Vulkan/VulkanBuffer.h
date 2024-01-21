#pragma once
#include "Rendering/Vertex.hpp"

class VulkanBuffer
{
public:
	static vk::BufferCreateInfo StagingCreateInfo(size_t inSize)
	{
		return vk::BufferCreateInfo()
			.setSize(inSize)
			.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
			.setSharingMode(vk::SharingMode::eExclusive);
	}

	static vk::BufferCreateInfo VertexBufferCreateInfo(const List<Vertex>& inVertices)
	{
		return vk::BufferCreateInfo()
			.setSize(inVertices.size() * sizeof(Vertex))
			.setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);
	}

	static vk::BufferCreateInfo IndexBufferCreateInfo(const List<uint>& inIndices)
	{
		return vk::BufferCreateInfo()
			.setSize(inIndices.size() * sizeof(uint))
			.setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer);

	}

	static vk::BufferCreateInfo UniformBufferCreateInfo(size_t inSize)
	{
		return vk::BufferCreateInfo()
			.setSize(inSize)
			.setUsage(vk::BufferUsageFlagBits::eUniformBuffer);
	}

public:
	vk::Buffer GetAPIResource() const;

	void SetData(void* inData, const size_t inSize);
	
	template<typename T>
	void SetData(T& inData)
	{
		static_assert(!std::is_pointer<T>::value && "Data type cannot be of pointer type");
		SetData(&inData, sizeof(T));
	}

	void* Map();
	void Unmap();

	size_t GetSize() const;

private:
	// Only create and destroy this resource via VulkanAllocator.
	VulkanBuffer() = default;
	~VulkanBuffer() = default;
	VulkanBuffer(const VulkanBuffer& inOther) = delete;

	void UploadMapped(void* inData, size_t inSize);
	void UploadStaged(void* inData, size_t inSize);

private:
	friend class VulkanAllocator;
	VmaAllocation myAllocation;
	vk::Buffer myBuffer;

	bool myIsMappingAllowed = false;

#ifdef DEBUG
	std::string myName = "";
#endif
};