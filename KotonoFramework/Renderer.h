#pragma once
#include "RenderQueue3D.h"
#include "ImageTexture.h"

constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

class KtRenderer final
{
public:
	void Init();
	void Cleanup();

	void AddToRenderQueue(const KtShader* shader, const KtModel* model);
	void DrawFrame();

	void RecreateSwapChain();
	void OnFramebufferResized();

	const VkExtent2D GetSwapChainExtent() const;

	VkRenderPass& GetRenderPass();
	VkDescriptorSetLayout& GetDescriptorSetLayout();
	VkPipelineLayout& GetPipelineLayout();

private:
	void CreateShaderAndModels();
	std::unordered_map<const KtShader*, std::unordered_set<const KtModel*>> _renderQueue3D;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	std::vector<VkFramebuffer> _swapChainFramebuffers;

	VkRenderPass _renderPass;
	VkDescriptorSetLayout _descriptorSetLayout;
	VkPipelineLayout _pipelineLayout;

	std::vector<VkCommandBuffer> _commandBuffers;

	VkImage _colorImage;
	VmaAllocation _colorImageAllocation;
	VkImageView _colorImageView;

	VkImage _depthImage;
	VmaAllocation _depthImageAllocation;
	VkImageView _depthImageView;

	std::array<VkBuffer, MAX_FRAMES_IN_FLIGHT> _uniformBuffers;
	std::array<VmaAllocation, MAX_FRAMES_IN_FLIGHT> _uniformBuffersAllocation;
	std::array<void*, MAX_FRAMES_IN_FLIGHT> _uniformBuffersMapped;
	VkDescriptorPool _descriptorPool;
	std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _descriptorSets;

	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;

	bool _framebufferResized = false;

	uint32_t _currentFrame = 0;


	KtImageTexture* _imageTexture;

	void CreateImageTexture();


	void CreateSwapChain();
	const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	const VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImageViews();

	void CreateRenderPass();
	void CreateFramebuffers();

	void CreateDescriptorSetLayout();

	void CreateColorResources();
	void CreateDepthResources();
	const VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
	const VkFormat FindDepthFormat() const;
	const bool HasStencilComponent(VkFormat format) const;
	void CreateUniformBuffers();
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) const;

	void BindShader(VkCommandBuffer commandBuffer, const KtShader* shader) const;
	void DrawModel(VkCommandBuffer commandBuffer, const KtModel* model) const;

	void CreateSyncObjects();

	void CleanupSwapChain() const;
};

