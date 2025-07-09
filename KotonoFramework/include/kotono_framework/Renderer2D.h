#pragma once
#include "UniformData2D.h"
#include "frames_in_flight.h"
#include "AllocatedBuffer.h"
#include <vulkan/vulkan_core.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>
class KtShader;
struct KtRenderable2DProxy;
class KtRenderer2D final
{
private:
	using ProxiesVector = std::vector<KtRenderable2DProxy*>;
	using ProxiesUnorderedSet = std::unordered_set<KtRenderable2DProxy*>;

public:
	void Init();
	void Update(const uint32_t frameIndex);
	void Cleanup() const;

	void SetUniformData(const KtUniformData2D& uniformData);

	void Register(KtRenderable2DProxy* proxy);
	void Remove(KtRenderable2DProxy* proxy);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

private:
	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;

	FramesInFlightArray<KtUniformData2D> uniformDatas_;

	FramesInFlightArray<VkCommandBuffer> commandBuffers_;
	FramesInFlightArray<bool> isCommandBufferDirty_;

	std::unordered_map<KtRenderable2DProxy*, int32_t> stagingProxies_;
	FramesInFlightArray<ProxiesUnorderedSet> proxies_;
	FramesInFlightArray<ProxiesVector> sortedProxies_;

	FramesInFlightArray<std::unordered_map<const KtShader*, uint32_t>> instanceIndices_;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const;
	void CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const;

	void CreateCommandBuffers();
	void RecordCommandBuffer(const uint32_t frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);
	
	void UpdateDescriptorSets(const ProxiesVector& renderQueueData, const uint32_t frameIndex);

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesVector& proxies, const uint32_t frameIndex);
	
	const ProxiesVector GetSortedProxies(const ProxiesUnorderedSet& proxies) const;
	const bool GetIsDynamicProxiesDirty(const uint32_t frameIndex) const;
};

