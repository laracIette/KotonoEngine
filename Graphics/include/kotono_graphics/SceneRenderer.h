#pragma once
#include "frames_in_flight.h"
#include "Mobility.h"
#include "RendererFrameStats.h"
#include "SceneUniformData.h"
#include "Scissor.h"
#include <kotono_common/Pool.h>
#include <unordered_map>
#include <vulkan/vulkan_core.h>
class USceneProxy;
class KtShader;
class KtSceneRenderable;
class KtSceneRenderer final
{
private:
	using Proxy = USceneProxy;
	using ProxiesPool = KtPool<Proxy*>;
	using StagingProxiesMap = std::unordered_map<Proxy*, int32_t>;
	using DeleteProxiesMap = std::unordered_map<Proxy*, uint32_t>;

public:
	void Init();
	void Update(const uint32_t frameIndex);
	void Cleanup();

	void MarkUniformBuffersDirty();
	void MarkObjectBuffersDirty();

	void SetUniformData(const KtSceneUniformData& uniformData);

	void RegisterProxy(Proxy* proxy, const EMobility mobility);
	void UnregisterProxy(Proxy* proxy, const EMobility mobility);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	Proxy* CreateProxy() const;
	// Stage deletion of the proxy after KT_FRAMES_IN_FLIGHT frames, 
	// warning: MUST be called after UnregisterProxy
	void DeleteProxy(Proxy* proxy);

private:
	struct DrawBatch
	{
		KtShader* shader;
		KtSceneRenderable* renderable;
		KtScissor scissor;
		uint32_t firstInstance;
		uint32_t instanceCount;
	};

	struct FrameData
	{
		struct ObjectBufferData
		{
			ProxiesPool proxies;
			ProxiesPool sortedProxies;
			std::vector<DrawBatch> drawBatches;
			VkCommandBuffer commandBuffer;
			bool isDirty;
		};

		struct UniformBufferData
		{
			KtSceneUniformData uniformData;
			bool isDirty;
		};

		KtRendererFrameStats stats;

		UniformBufferData uniformBuffer;

		ObjectBufferData staticBuffer;
		ObjectBufferData dynamicBuffer;

		std::unordered_map<const KtShader*, uint32_t> instanceIndices;
	};

	// Those are accessed from one thread
	std::pair<KtSceneUniformData, uint32_t> stagingUniformData_;
	StagingProxiesMap stagingStaticProxies_;
	StagingProxiesMap stagingDynamicProxies_;
	DeleteProxiesMap deleteProxies_;

	// Those are accessed from multiple threads
	KtFramesInFlightArray<FrameData> frameDatas_;

	void CreateCommandBuffers();
	void CreateCommandBuffer(FrameData::ObjectBufferData& objectBuffer, const uint32_t frameIndex);
	void RecordCommandBuffer(const FrameData::ObjectBufferData& objectBuffer, const uint32_t frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);

	void UpdateUniformData(const uint32_t frameIndex);
	void UpdateProxies(FrameData::ObjectBufferData& objectBuffer, const uint32_t frameIndex);
	void UpdateStagingProxies(StagingProxiesMap& stagingProxies, FrameData::ObjectBufferData& objectBuffer, const uint32_t frameIndex);
	void UpdateDescriptorSetObjectBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const;
	void UpdateDescriptorSetUniformBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const;

	void DeleteProxies();

	void SortProxies(ProxiesPool& proxies, const uint32_t frameIndex);

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const std::vector<DrawBatch>& drawBatches, const uint32_t frameIndex);
	void CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	std::vector<DrawBatch> GetDrawBatches(const ProxiesPool& proxies, const uint32_t frameIndex);
	void UpdateIndirectBuffers(const std::vector<DrawBatch>& drawBatches, const uint32_t frameIndex);
};
