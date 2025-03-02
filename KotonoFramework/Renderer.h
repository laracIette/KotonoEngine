#pragma once
#include "RenderQueue3D.h"
#include "ImageTexture.h"
#include "max_frames_in_flight.h"
#include "ObjectData3D.h"

class KtRenderer final
{
public:
	void Init();
	void Cleanup();

	void AddToRenderQueue(KtShader* shader, KtModel* model, const KtObjectData3D& objectData);
	void DrawFrame();

	void RecreateSwapChain();
	void OnFramebufferResized();

	const VkExtent2D GetSwapChainExtent() const;
	const uint32_t GetCurrentFrame() const;

	VkRenderPass GetRenderPass() const;
	//VkDescriptorSetLayout& GetDescriptorSetLayout();

private:
	void CreateShaderAndModels();
	std::unordered_map<KtShader*, std::unordered_map<KtModel*, std::vector<KtObjectData3D>>> _renderQueue3D;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	std::vector<VkFramebuffer> _swapChainFramebuffers;

	VkRenderPass _renderPass;
	//VkDescriptorSetLayout _descriptorSetLayout;

	std::vector<VkCommandBuffer> _commandBuffers;

	VkImage _colorImage;
	VmaAllocation _colorImageAllocation;
	VkImageView _colorImageView;

	VkImage _depthImage;
	VmaAllocation _depthImageAllocation;
	VkImageView _depthImageView;

	//std::array<VkBuffer, MAX_FRAMES_IN_FLIGHT> _uniformBuffers;
	//std::array<VmaAllocation, MAX_FRAMES_IN_FLIGHT> _uniformBuffersAllocation;
	//std::array<void*, MAX_FRAMES_IN_FLIGHT> _uniformBuffersMapped;
	//VkDescriptorPool _descriptorPool;
	//std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _globalDescriptorSets;

	std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> _imageAvailableSemaphores;
	std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> _renderFinishedSemaphores;
	std::array<VkFence, MAX_FRAMES_IN_FLIGHT> _inFlightFences;

	bool _framebufferResized = false;

	uint32_t _currentFrame = 0;


	//KtImageTexture* _imageTexture;

	void CreateImageTexture();


	void CreateSwapChain();
	const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	const VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImageViews();

	void CreateRenderPass();
	void CreateFramebuffers();

	//void CreateDescriptorSetLayout();

	void CreateColorResources();
	void CreateDepthResources();
	const VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
	const VkFormat FindDepthFormat() const;
	const bool HasStencilComponent(VkFormat format) const;
	//void CreateUniformBuffers();
	//void UpdateUniformBuffer(const uint32_t imageIndex);
	//void CreateDescriptorPool();
	//void CreateDescriptorSets();

	//void UpdateDescriptorSet(const uint32_t imageIndex, const KtImageTexture* imageTexture);

	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t imageIndex) const;

	//void CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const KtPushConstantData& pushConstantData) const;

	void CmdBindModel(VkCommandBuffer commandBuffer, KtModel* model) const;
	void CmdDrawModel(VkCommandBuffer commandBuffer, KtModel* model, const uint32_t instanceCount) const;

	void CreateSyncObjects();

	void CleanupSwapChain() const;
};

