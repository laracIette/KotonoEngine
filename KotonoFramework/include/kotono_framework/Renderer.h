#pragma once
#include "ImageTexture.h"
#include "frames_in_flight.h"
#include "ObjectData2D.h"
#include "ObjectData3D.h"
#include "Shader2D.h"
#include "Shader3D.h"
#include "Model.h"
#include <vector>
#include <array>
#include <unordered_map>
#include "RenderLayer.h"

class KtRenderer final
{
public:
	void Init();
	void Cleanup();

	template <KtRenderLayer Layer>
	void AddToRenderQueue3D(KtShader3D* shader, KtModel* model, const KtObjectData3D& objectData);
	void SetUniformData3D(const KtUniformData3D& uniformData3D);

	template <KtRenderLayer Layer>
	void AddToRenderQueue2D(KtShader2D* shader, const KtObjectData2D& objectData);
	void SetUniformData2D(const KtUniformData2D& uniformData2D);

	void DrawFrame();

	void OnFramebufferResized();

	const VkExtent2D GetSwapChainExtent() const;

	VkRenderPass GetRenderPass() const;

private:
	void CreateShaderAndModels() const;

	struct RenderQueue2DModelData
	{
		std::vector<KtObjectData2D> ObjectDatas;
	};
	struct RenderQueue2DData
	{
		std::unordered_map<KtShader2D*, RenderQueue2DModelData> Shaders;
	};
	struct Renderer2DData
	{
		std::array<RenderQueue2DData, KT_RENDER_LAYER_COUNT> RenderQueues;
	};
	Renderer2DData _renderer2DData;
	KtUniformData2D _uniformData2D;

	struct RenderQueue3DModelData
	{
		std::vector<KtObjectData3D> ObjectDatas;
	};
	struct RenderQueue3DShaderData
	{
		std::unordered_map<KtModel*, RenderQueue3DModelData> Models;
	};
	struct RenderQueue3DData
	{
		std::unordered_map<KtShader3D*, RenderQueue3DShaderData> Shaders;
	};
	struct Renderer3DData
	{
		std::array<RenderQueue3DData, KT_RENDER_LAYER_COUNT> RenderQueues;
	};
	Renderer3DData _renderer3DData;
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

	std::array<VkSemaphore, KT_FRAMES_IN_FLIGHT> _imageAvailableSemaphores;
	std::array<VkSemaphore, KT_FRAMES_IN_FLIGHT> _renderFinishedSemaphores;
	std::array<VkFence, KT_FRAMES_IN_FLIGHT> _inFlightFences;

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

	void ClearRenderQueues();
	void ClearRenderQueue2D();
	void ClearRenderQueue3D();

	void CleanupSwapChain() const;
};