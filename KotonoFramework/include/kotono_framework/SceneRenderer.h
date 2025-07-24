#pragma once
#include "UniformData3D.h"
#include "frames_in_flight.h"
#include "Pool.h"
#include "RendererFrameStats.h"
#include <vulkan/vulkan_core.h>
struct KtRenderable3DProxy;
class KtShader;
class KtSceneRenderer final
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
	void UnregisterStatic(KtRenderable3DProxy* proxy);
	void UnregisterDynamic(KtRenderable3DProxy* proxy);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

private:
	KtFramesInFlightArray<KtRendererFrameStats> stats_;

	std::pair<KtUniformData3D, uint32_t> stagingUniformData_;
	KtFramesInFlightArray<KtUniformData3D> uniformDatas_;
	KtFramesInFlightArray<bool> isUniformBufferDirty_;

	KtFramesInFlightArray<VkCommandBuffer> staticCommandBuffers_;
	KtFramesInFlightArray<VkCommandBuffer> dynamicCommandBuffers_;
	KtFramesInFlightArray<bool> isStaticCommandBufferDirty_;
	KtFramesInFlightArray<bool> isDynamicCommandBufferDirty_;

	std::unordered_map<KtRenderable3DProxy*, int32_t> stagingStaticProxies_;
	std::unordered_map<KtRenderable3DProxy*, int32_t> stagingDynamicProxies_;
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
	const bool GetIsDynamicProxiesDirty(const uint32_t frameIndex) const;
	void MarkDynamicProxiesNotDirty(const uint32_t frameIndex);
};

