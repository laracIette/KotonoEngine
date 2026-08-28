#pragma once
#include <vulkan/vulkan_core.h>
class UContext final
{
public:
	void Init();
	void Cleanup() const;

	VkInstance GetInstance() const { return instance_; }

private:
	void CreateInstance();
	void SetupDebugMessenger();

private:
	VkInstance instance_;
	VkDebugUtilsMessengerEXT debugMessenger_;
};
