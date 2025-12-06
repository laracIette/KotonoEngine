#pragma once
#include "SceneUniformData.h"
#include "frames_in_flight.h"
#include <kotono_common/Pool.h>
#include "RendererFrameStats.h"
#include <vulkan/vulkan_core.h>
struct KtSceneRenderableProxy;
class KtShader;
class KtSceneRenderer final
{
private:
	using ProxiesPool = KtPool<KtSceneRenderableProxy*>;

public:
	void Init();
	void Update(const uint32_t frameIndex);
	void Cleanup();

	void SetUniformData(const KtSceneUniformData& uniformData);

	void RegisterStatic(KtSceneRenderableProxy* proxy);
	void RegisterDynamic(KtSceneRenderableProxy* proxy);
	void UnregisterStatic(KtSceneRenderableProxy* proxy);
	void UnregisterDynamic(KtSceneRenderableProxy* proxy);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

private:
	KtFramesInFlightArray<KtRendererFrameStats> stats_;

	std::pair<KtSceneUniformData, uint32_t> stagingUniformData_;
	KtFramesInFlightArray<KtSceneUniformData> uniformDatas_;
	KtFramesInFlightArray<bool> isUniformBufferDirty_;

	KtFramesInFlightArray<VkCommandBuffer> staticCommandBuffers_;
	KtFramesInFlightArray<VkCommandBuffer> dynamicCommandBuffers_;
	KtFramesInFlightArray<bool> isStaticCommandBufferDirty_;
	KtFramesInFlightArray<bool> isDynamicCommandBufferDirty_;

	std::unordered_map<KtSceneRenderableProxy*, int32_t> stagingStaticProxies_;
	std::unordered_map<KtSceneRenderableProxy*, int32_t> stagingDynamicProxies_;
	KtFramesInFlightArray<ProxiesPool> staticProxies_;
	KtFramesInFlightArray<ProxiesPool> dynamicProxies_;

	KtFramesInFlightArray<std::unordered_map<const KtShader*, uint32_t>> instanceIndices_;

	void CreateStaticCommandBuffers();
	void CreateStaticCommandBuffer(const uint32_t frameIndex);
	void CreateDynamicCommandBuffers();
	void CreateDynamicCommandBuffer(const uint32_t frameIndex);
	void RecordStaticCommandBuffer(const uint32_t frameIndex);
	void RecordDynamicCommandBuffer(const uint32_t frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);

	void UpdateUniformData(const uint32_t frameIndex);

	void UpdateStaticProxies(const uint32_t frameIndex);
	void UpdateDynamicProxies(const uint32_t frameIndex);
	void UpdateDescriptorSetObjectBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const;
	void UpdateDescriptorSetUniformBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const;

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesPool& proxies, const uint32_t frameIndex);
	void CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	void SortProxies(ProxiesPool& proxies);
	bool GetIsDynamicProxiesDirty(const uint32_t frameIndex) const;
	void MarkDynamicProxiesNotDirty(const uint32_t frameIndex);
};

