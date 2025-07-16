#pragma once
#include "UniformData2D.h"
#include "frames_in_flight.h"
#include "AllocatedBuffer.h"
#include "Pool.h"
#include <vulkan/vulkan_core.h>
#include <vector>
#include <unordered_map>
class KtShader;
struct KtRenderable2DProxy;
class KtRenderer2D final
{
private:
	using ProxiesPool = KtPool<KtRenderable2DProxy*>;

public:
	void Init();
	void Update(const uint32_t frameIndex);
	void Cleanup() const;

	void SetUniformData(const KtUniformData2D& uniformData);

	void Register(KtRenderable2DProxy* proxy);
	void Unregister(KtRenderable2DProxy* proxy);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

private:
	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;
	KtAllocatedBuffer stagingVertexBuffer_;
	KtAllocatedBuffer stagingIndexBuffer_;

	KtFramesInFlightArray<KtUniformData2D> uniformDatas_;

	KtFramesInFlightArray<VkCommandBuffer> commandBuffers_;
	KtFramesInFlightArray<bool> isCommandBufferDirty_;

	std::unordered_map<KtRenderable2DProxy*, int32_t> stagingProxies_;
	KtFramesInFlightArray<ProxiesPool> proxies_;

	KtFramesInFlightArray<std::unordered_map<const KtShader*, uint32_t>> instanceIndices_;

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void DestroyStagingVertexBuffer();
	void DestroyStagingIndexBuffer();

	void CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const;
	void CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const;

	void CreateCommandBuffers();
	void CreateCommandBuffer(const uint32_t frameIndex);
	void RecordCommandBuffer(const uint32_t frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);
	
	void UpdateDescriptorSets(const ProxiesPool& renderQueueData, const uint32_t frameIndex);

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesPool& proxies, const uint32_t frameIndex);
	
	void SortProxies(ProxiesPool& proxies);
	const bool GetIsAnyProxyDirty(const uint32_t frameIndex) const;
	void MarkProxiesNotDirty(const uint32_t frameIndex);
};

