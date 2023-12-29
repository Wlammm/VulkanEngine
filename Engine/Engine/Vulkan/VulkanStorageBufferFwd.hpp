#pragma once

class VulkanBuffer;

class IVulkanStorageBuffer
{
public:
	virtual vk::ShaderStageFlags GetShaderStageFlags() const = 0;
	virtual uint GetBindingIndex() const = 0;
	virtual vk::Buffer GetBuffer() const = 0;
	virtual size_t GetBufferSize() const = 0;
};

template<typename T>
class VulkanStorageBuffer : public IVulkanStorageBuffer
{
public:
	VulkanStorageBuffer(const vk::ShaderStageFlags inShaderStageFlags, const uint inBindingIndex);
	~VulkanStorageBuffer();

	vk::ShaderStageFlags GetShaderStageFlags() const override final;
	vk::Buffer GetBuffer() const override final;
	virtual size_t GetBufferSize() const override final;
	uint GetBindingIndex() const override final;
	void SetData(const T& inData);

private:
	void CreateBuffers(const size_t inSize);
	void InvalidateBuffer();

private:
	vk::ShaderStageFlags myShaderStages;
	uint myBindingIndex;
	VulkanBuffer* myBuffer = nullptr;
	size_t mySize;
};