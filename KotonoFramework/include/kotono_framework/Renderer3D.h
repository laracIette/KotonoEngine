#pragma once
#include "UniformData3D.h"
#include "frames_in_flight.h"
#include "Pool.h"
#include "RendererFrameStats.h"
#include <vulkan/vulkan_core.h>
struct KtRenderable3DProxy;
class KtShader;
class KtRenderer3D final
{
private:
	using ProxiesPool = KtPool<KtRenderable3DProxy*>;

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
	FramesInFlightArray<KtUniformData3D> uniformDatas_;
	FramesInFlightArray<KtRendererFrameStats> stats_;

	FramesInFlightArray<VkCommandBuffer> staticCommandBuffers_;
	FramesInFlightArray<VkCommandBuffer> dynamicCommandBuffers_;
	FramesInFlightArray<bool> isStaticCommandBufferDirty_;
	FramesInFlightArray<bool> isDynamicCommandBufferDirty_;

	std::unordered_map<KtRenderable3DProxy*, int32_t> stagingStaticProxies_;
	std::unordered_map<KtRenderable3DProxy*, int32_t> stagingDynamicProxies_;
	FramesInFlightArray<ProxiesPool> staticProxies_;
	FramesInFlightArray<ProxiesPool> dynamicProxies_;

	FramesInFlightArray<std::unordered_map<const KtShader*, uint32_t>> instanceIndices_;

	void CreateStaticCommandBuffers();
	void CreateDynamicCommandBuffers();
	void RecordStaticCommandBuffer(const uint32_t frameIndex);
	void RecordDynamicCommandBuffer(const uint32_t frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);

	void UpdateStaticProxies(const uint32_t frameIndex);
	void UpdateDynamicProxies(const uint32_t frameIndex);
	void UpdateDescriptorSets(const ProxiesPool& proxies, const uint32_t frameIndex) const;

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesPool& proxies, const uint32_t frameIndex);
	void CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	void SortProxies(ProxiesPool& proxies);
	const bool GetIsDynamicProxiesDirty(const uint32_t frameIndex) const;
	void MarkDynamicProxiesNotDirty(const uint32_t frameIndex);
};

