#pragma once
#include "ImageTexture.h"
#include "max_frames_in_flight.h"
#include "ObjectData3D.h"
#include "Shader.h"
#include "Model.h"
#include <vector>
#include <array>
#include <unordered_map>

class KtRenderer final
{
public:
	void Init();
	void Cleanup();

	void AddToRenderQueue3D(KtShader* shader, KtModel* model, const KtObjectData3D& objectData);
	void SetUniformData3D(const KtUniformData3D& uniformData3D);

	void DrawFrame();

	void OnFramebufferResized();

	const VkExtent2D GetSwapChainExtent() const;

	VkRenderPass GetRenderPass() const;

private:
	void CreateShaderAndModels() const;

	struct RenderQueue3DModelData
	{
		std::vector<KtObjectData3D> ObjectDatas;
	};
	struct RenderQueue3DShaderData
	{
		std::unordered_map<KtModel*, RenderQueue3DModelData> Models;
	};
	struct RenderQueue3D
	{
		std::unordered_map<KtShader*, RenderQueue3DShaderData> Shaders;
	};

	RenderQueue3D _renderQueue3D;
	KtUniformData3D _uniformData3D;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	std::vector<VkFramebuffer> _swapChainFramebuffers;

	VkRenderPass _renderPass;

	std::vector<VkCommandBuffer> _commandBuffers;

	VkImage _colorImage;
	VmaAllocation _colorImageAllocation;
	VkImageView _colorImageView;

	VkImage _depthImage;
	VmaAllocation _depthImageAllocation;
	VkImageView _depthImageView;

	std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> _imageAvailableSemaphores;
	std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> _renderFinishedSemaphores;
	std::array<VkFence, MAX_FRAMES_IN_FLIGHT> _inFlightFences;

	bool _framebufferResized;

	uint32_t _currentFrame;

	void CreateSwapChain();
	void RecreateSwapChain();
	const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	const VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImageViews();

	void CreateRenderPass();
	void CreateFramebuffers();

	void CreateColorResources();
	void CreateDepthResources();
	const VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
	const VkFormat FindDepthFormat() const;
	const bool HasStencilComponent(VkFormat format) const;

	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t imageIndex) const;

	void CreateSyncObjects();

	void ClearRenderQueue();

	void CleanupSwapChain() const;
};

