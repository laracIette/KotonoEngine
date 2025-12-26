#pragma once
#include "frames_in_flight.h"
#include "Mobility.h"
#include "RendererFrameStats.h"
#include "SceneUniformData.h"
#include <kotono_common/Pool.h>
#include <unordered_map>
#include <unordered_set>
#include <vulkan/vulkan_core.h>
class USceneProxy;
class KtShader;
class KtSceneRenderer final
{
private:
	using Proxy = USceneProxy;
	using ProxiesPool = KtPool<Proxy*>;

public:
	void Init();
	void Update(const uint32_t frameIndex);
	void Cleanup();

	void MarkCommandBuffersDirty();

	void SetUniformData(const KtSceneUniformData& uniformData);

	void RegisterProxy(Proxy* proxy, const EMobility mobility);
	void UnregisterProxy(Proxy* proxy, const EMobility mobility);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	Proxy* CreateProxy() const;

private:
	KtFramesInFlightArray<KtRendererFrameStats> stats_;

	std::pair<KtSceneUniformData, uint32_t> stagingUniformData_;
	KtFramesInFlightArray<KtSceneUniformData> uniformDatas_;
	KtFramesInFlightArray<bool> isUniformBufferDirty_;

	std::unordered_map<Proxy*, int32_t> stagingStaticProxies_;
	std::unordered_map<Proxy*, int32_t> stagingDynamicProxies_;
	KtFramesInFlightArray<ProxiesPool> staticProxies_;
	KtFramesInFlightArray<ProxiesPool> dynamicProxies_;
	KtFramesInFlightArray<ProxiesPool> sortedStaticProxies_;
	KtFramesInFlightArray<ProxiesPool> sortedDynamicProxies_;

	KtFramesInFlightArray<VkCommandBuffer> staticCommandBuffers_;
	KtFramesInFlightArray<VkCommandBuffer> dynamicCommandBuffers_;
	KtFramesInFlightArray<bool> isStaticCommandBufferDirty_;
	KtFramesInFlightArray<bool> isDynamicCommandBufferDirty_;

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
	void UpdateStagingStaticProxies(const uint32_t frameIndex);
	void UpdateStagingDynamicProxies(const uint32_t frameIndex);
	void UpdateDescriptorSetObjectBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const;
	void UpdateDescriptorSetUniformBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const;

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesPool& proxies, const uint32_t frameIndex);
	void CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	void SortProxies(ProxiesPool& proxies, const uint32_t frameIndex);
};

