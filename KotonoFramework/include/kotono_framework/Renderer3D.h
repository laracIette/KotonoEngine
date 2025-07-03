#pragma once
#include "UniformData3D.h"
#include "RenderQueue3DData.h"
#include "frames_in_flight.h"
#include <vulkan/vulkan_core.h>
#include <array>
#include <unordered_set>
#include "RendererFrameStats.h"
struct KtRenderable3DProxy;
class KtRenderer3D final
{
public:
	void Init();
	void Cleanup();

	void SetUniformData(const KtUniformData3D& uniformData);

	void Register(KtRenderable3DProxy* proxy);
	void Remove(KtRenderable3DProxy* proxy);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame);
	
	void Reset(const uint32_t currentFrame);

private:
	std::array<KtRenderQueue3DData, KT_FRAMES_IN_FLIGHT> renderQueueData_;
	std::array<KtUniformData3D, KT_FRAMES_IN_FLIGHT> uniformData_;
	std::array<KtRendererFrameStats, KT_FRAMES_IN_FLIGHT> stats_;

	std::unordered_set<KtRenderable3DProxy*> proxys_; // todo: array?

	void UpdateProxys();
	void AddProxyToRenderQueue(KtRenderable3DProxy* proxy, const uint32_t currentFrame);
	void RemoveProxyFromRenderQueue(KtRenderable3DProxy* proxy, const uint32_t currentFrame);

	void CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame);
};

