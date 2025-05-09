#pragma once
#include "Engine/Delegates/MulticastDelegate.hpp"
#include "Engine/Rendering/Vertex.hpp"

class VulkanBuffer
{
public:
	static vk::BufferCreateInfo StagingCreateInfo(uint inSize)
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

	static vk::BufferCreateInfo UniformBufferCreateInfo(uint inSize)
	{
		return vk::BufferCreateInfo()
			.setSize(inSize)
			.setUsage(vk::BufferUsageFlagBits::eUniformBuffer);
	}

	static vk::BufferCreateInfo StorageBufferCreateInfo(uint inSize)
	{
		return vk::BufferCreateInfo()
			.setSize(inSize)
			.setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst)
			.setSharingMode(vk::SharingMode::eExclusive);
	}

	static vk::BufferCreateInfo ResizableStorageBufferCreateInfo(uint inSize)
	{
		return vk::BufferCreateInfo()
			.setSize(inSize)
			.setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc)
			.setSharingMode(vk::SharingMode::eExclusive);
	}
	
public:
	vk::Buffer GetAPIResource() const;

	void CopyDataFromBuffer(VulkanBuffer* inStagingBuffer, const uint inSize, uint inOffset);
	void SetData(const void* inData, const uint inSize, uint inOffset = 0);

	template<typename T>
	void SetData(const T& inData)
	{
		static_assert(!std::is_pointer<T>::value && "Data type cannot be of pointer type");
		SetData(&inData, sizeof(T));
	}

	uint GetSize() const;
	const vk::BufferCreateInfo& GetCreateInfo() const;
	VmaMemoryUsage GetVmaMemoryUsage() const;
	bool IsMappable() const;

#if DEBUG
	const std::string& GetName() const;
#else
	std::string GetName() const;
#endif
	
	void* GetPtr() const;
	
private:
	// Only create and destroy this resource via VulkanAllocator.
	VulkanBuffer() = default;
	~VulkanBuffer() = default;
	VulkanBuffer(const VulkanBuffer& inOther) = delete;

	void UploadMapped(const void* inData, uint inSize, uint inOffset);
	void UploadStaged(const void* inData, uint inSize, uint inOffset);

private:
	friend class VulkanAllocator;
	VmaAllocation myAllocation;
	vk::Buffer myBuffer;
	vk::BufferCreateInfo myCreateInfo;
	VmaMemoryUsage myMemoryUsage;

	bool myIsMappingAllowed = false;

	void* myPtr = nullptr;
	
#ifdef DEBUG
	std::string myName = "";
#endif
};