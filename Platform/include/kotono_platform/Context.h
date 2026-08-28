#pragma once
#include <kotono_common/types.h>
#include <vector>
#include <vulkan/vulkan_core.h>
class GContext final
{
public:
	void Init();
	void Cleanup() const;

	VkInstance GetInstance() const { return instance_; }

private:
	void CreateInstance();

	void SetupDebugMessenger();
	
	b8 CheckValidationLayerSupport();
	
	std::vector<const char*> GetRequiredExtensions();

private:
	VkInstance instance_;
	VkDebugUtilsMessengerEXT debugMessenger_;
};

inline GContext Context;
