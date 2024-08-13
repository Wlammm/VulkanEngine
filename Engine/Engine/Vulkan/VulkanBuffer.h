#pragma once
#include "Delegates/MulticastDelegate.hpp"
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

	static vk::BufferCreateInfo StorageBufferCreateInfo(size_t inSize)
	{
		return vk::BufferCreateInfo()
			.setSize(inSize)
			.setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst)
			.setSharingMode(vk::SharingMode::eExclusive);
	}

public:
	vk::Buffer GetAPIResource() const;

	void SetData(const void* inData, const size_t inSize, uint inOffset = 0);

	template<typename T>
	void SetData(const T& inData)
	{
		static_assert(!std::is_pointer<T>::value && "Data type cannot be of pointer type");
		SetData(&inData, sizeof(T));
	}

	void* Map();
	void Unmap();

	size_t GetSize() const;
	const vk::BufferCreateInfo& GetCreateInfo() const;
	VmaMemoryUsage GetVmaMemoryUsage() const;
	bool IsMappable() const;

	const std::string& GetName() const;
	
private:
	// Only create and destroy this resource via VulkanAllocator.
	VulkanBuffer() = default;
	~VulkanBuffer() = default;
	VulkanBuffer(const VulkanBuffer& inOther) = delete;

	void UploadMapped(const void* inData, size_t inSize, uint inOffset);
	void UploadStaged(const void* inData, size_t inSize, uint inOffset);

private:
	friend class VulkanAllocator;
	VmaAllocation myAllocation;
	vk::Buffer myBuffer;
	vk::BufferCreateInfo myCreateInfo;
	VmaMemoryUsage myMemoryUsage;

	bool myIsMappingAllowed = false;

#ifdef DEBUG
	std::string myName = "";
#endif
};