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
	void Update(const uint32_t frameIndex);
	void Cleanup();

	void SetUniformData(const KtUniformData3D& uniformData);

	void RegisterStatic(KtRenderable3DProxy* proxy);
	void RegisterDynamic(KtRenderable3DProxy* proxy);
	void RemoveStatic(KtRenderable3DProxy* proxy);
	void RemoveDynamic(KtRenderable3DProxy* proxy);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);


private:
	using ProxiesVector = std::vector<KtRenderable3DProxy*>;
	using ProxiesUnorderedSet = std::unordered_set<KtRenderable3DProxy*>;

	FramesInFlightArray<KtUniformData3D> uniformDatas_;
	FramesInFlightArray<KtRendererFrameStats> stats_;

	FramesInFlightArray<VkCommandBuffer> staticCommandBuffers_;
	FramesInFlightArray<VkCommandBuffer> dynamicCommandBuffers_;
	FramesInFlightArray<bool> isDynamicCommandBufferDirty_;
	FramesInFlightArray<bool> isStaticCommandBufferDirty_;

	std::unordered_map<KtRenderable3DProxy*, int32_t> stagingStaticProxies_;
	std::unordered_map<KtRenderable3DProxy*, int32_t> stagingDynamicProxies_;
	FramesInFlightArray<ProxiesUnorderedSet> globalProxies_;
	FramesInFlightArray<ProxiesUnorderedSet> staticProxies_;
	FramesInFlightArray<ProxiesUnorderedSet> dynamicProxies_;
	FramesInFlightArray<ProxiesVector> sortedGlobalProxies_;
	FramesInFlightArray<ProxiesVector> sortedStaticProxies_;
	FramesInFlightArray<ProxiesVector> sortedDynamicProxies_;

	FramesInFlightArray<std::unordered_map<const KtShader*, uint32_t>> instanceIndices_;

	void CreateStaticCommandBuffers();
	void CreateDynamicCommandBuffers();
	void RecordStaticCommandBuffer(const uint32_t frameIndex);
	void RecordDynamicCommandBuffer(const uint32_t frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);

	void UpdateStaticProxies(const uint32_t frameIndex);
	void UpdateDynamicProxies(const uint32_t frameIndex);
	void UpdateDescriptorSets(const ProxiesVector& proxies, const uint32_t frameIndex) const;

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesVector& proxies, const uint32_t frameIndex);
	void CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	const ProxiesVector GetSortedProxies(const ProxiesUnorderedSet& proxies) const;
	const bool GetIsDynamicProxiesDirty(const uint32_t frameIndex) const;
};

