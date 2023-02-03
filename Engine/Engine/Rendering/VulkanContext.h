#pragma once

class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();

private:
	void CheckValidationLayerSupport();
	void CreateVulkanInstance();

private:
	inline static VulkanContext* myInstance = nullptr;

	vk::Instance myVulkanInstance;


	const std::vector<const char*> myExtensions
	{

	};

	const std::vector<const char*> myLayers
	{ 
#ifdef DEBUG
		"VK_LAYER_KHRONOS_validation",
#endif
	};
};