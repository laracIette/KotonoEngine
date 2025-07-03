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

private:
	std::array<VkCommandBuffer, KT_FRAMES_IN_FLIGHT> staticCommandBuffers_;
	KtRenderQueue3DData staticRenderQueueData_;
	std::array<bool, KT_FRAMES_IN_FLIGHT> isStaticCommandBufferDirty_;

	std::array<VkCommandBuffer, KT_FRAMES_IN_FLIGHT> dynamicCommandBuffers_;

	std::array<KtRenderQueue3DData, KT_FRAMES_IN_FLIGHT> renderQueueData_;
	std::array<KtUniformData3D, KT_FRAMES_IN_FLIGHT> uniformData_;
	std::array<KtRendererFrameStats, KT_FRAMES_IN_FLIGHT> stats_;

	std::unordered_set<KtRenderable3DProxy*> proxys_; // todo: array?

	std::array<std::unordered_map<KtShader*, uint32_t>, KT_FRAMES_IN_FLIGHT> instanceIndices_;

	void CreateStaticCommandBuffers();
	void CreateDynamicCommandBuffers();
	void RecordStaticCommandBuffer(const uint32_t currentFrame);
	void RecordDynamicCommandBuffer(const uint32_t currentFrame);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t currentFrame);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);

	void UpdateProxys();
	void AddProxyToRenderQueue(KtRenderable3DProxy* proxy, const uint32_t currentFrame);
	void RemoveProxyFromRenderQueue(KtRenderable3DProxy* proxy, const uint32_t currentFrame);

	void UpdateDescriptorSets(const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame);
	void CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame);
};

