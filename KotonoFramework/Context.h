#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vk_mem_alloc.h> 
#include "glm_includes.h"
#include <stb_image.h>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"
#include "Vertex.h"
#include "ImageTexture.h"
#include "Model.h"

class KtContext final
{
public:
	void Init();
	void Cleanup() const;
	void DrawFrame();

	void OnFramebufferResized();

	VkPhysicalDevice GetPhysicalDevice() const;
	VkDevice GetDevice() const;
	VmaAllocator GetAllocator() const;

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VmaAllocationCreateFlags flags, VkBuffer& buffer, VmaAllocation& allocation, VmaAllocationInfo& allocInfo) const;
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VmaAllocation& imageAllocation) const;
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	const VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) const;

private:
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	// Window surface
	VkSurfaceKHR _surface;

	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	// Logical device
	VkDevice _device;

	VmaAllocator _allocator;

	VkQueue _graphicsQueue;
	// Presentation queue
	VkQueue _presentQueue;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;

	VkRenderPass _renderPass;
	VkDescriptorSetLayout _descriptorSetLayout;
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	std::vector<VkFramebuffer> _swapChainFramebuffers;
	VkCommandPool _commandPool;
	std::vector<VkCommandBuffer> _commandBuffers;

	KtModel* _model;

	std::vector<VkBuffer> _uniformBuffers;
	std::vector<VmaAllocation> _uniformBuffersAllocation;
	std::vector<void*> _uniformBuffersMapped;
	VkDescriptorPool _descriptorPool;
	std::vector<VkDescriptorSet> _descriptorSets;

	KtImageTexture* _imageTexture;

	VkSampleCountFlagBits _msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage _colorImage;
	VmaAllocation _colorImageAllocation;
	VkImageView _colorImageView;

	VkImage _depthImage;
	VmaAllocation _depthImageAllocation;
	VkImageView _depthImageView;

	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;

	bool _framebufferResized = false;

	uint32_t _currentFrame = 0;

	void CreateInstance();

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;
	void SetupDebugMessenger();
	const bool CheckValidationLayerSupport();
	const std::vector<const char*> GetRequiredExtensions();
	void PickPhysicalDevice();
	const bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	const KtQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
	void CreateLogicalDevice();
	void CreateSurface();
	const KtSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
	const VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const;
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) const;
	const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	const VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateSwapChain();
	void CreateImageViews();

	void CreateAllocator();

	void CreateDescriptorSetLayout();
	void CreateGraphicsPipeline();
	const VkShaderModule CreateShaderModule(const std::vector<char>& code) const;
	void CreateRenderPass();

	void CreateFramebuffers();
	const VkCommandBuffer BeginSingleTimeCommands() const;
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;
	void CreateCommandPool();
	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) const;
	const uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

	void CreateModel();
	void CreateUniformBuffers();
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	void CreateSyncObjects();

	void CreateDepthResources();
	const VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
	const VkFormat FindDepthFormat() const;
	const bool HasStencilComponent(VkFormat format) const;

	void CreateImageTexture();

	const VkSampleCountFlagBits GetMaxUsableSampleCount() const;
	void CreateColorResources();

	void RecreateSwapChain();
	void CleanupSwapChain() const;

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};
