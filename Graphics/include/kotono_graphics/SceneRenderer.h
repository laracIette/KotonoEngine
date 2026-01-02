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
	using StagingProxiesMap = std::unordered_map<Proxy*, i32>;
	using DeleteProxiesMap = std::unordered_map<Proxy*, u32>;

public:
	void Init();
	void Update(const u32 frameIndex);
	void Cleanup();

	void MarkUniformBuffersDirty();
	void MarkObjectBuffersDirty();

	void SetUniformData(const KtSceneUniformData& uniformData);

	void RegisterProxy(Proxy* proxy, const EMobility mobility);
	void UnregisterProxy(Proxy* proxy, const EMobility mobility);

	void CmdDraw(VkCommandBuffer commandBuffer, const u32 frameIndex);

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
		u32 firstInstance;
		u32 instanceCount;
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

		std::unordered_map<const KtShader*, u32> instanceIndices;
	};

	// Those are accessed from one thread
	std::pair<KtSceneUniformData, u32> stagingUniformData_;
	StagingProxiesMap stagingStaticProxies_;
	StagingProxiesMap stagingDynamicProxies_;
	DeleteProxiesMap deleteProxies_;

	// Those are accessed from multiple threads
	KtFramesInFlightArray<FrameData> frameDatas_;

	void CreateCommandBuffers();
	void CreateCommandBuffer(FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex);
	void RecordCommandBuffer(const FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const u32 frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);

	void UpdateUniformData(const u32 frameIndex);
	void UpdateProxies(FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex);
	void UpdateStagingProxies(StagingProxiesMap& stagingProxies, FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex);
	void UpdateDescriptorSetObjectBuffers(const ProxiesPool& proxies, const u32 frameIndex) const;
	void UpdateDescriptorSetUniformBuffers(const ProxiesPool& proxies, const u32 frameIndex) const;

	void DeleteProxies();

	void SortProxies(FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex);

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const std::vector<DrawBatch>& drawBatches, const u32 frameIndex);
	void CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const u32 frameIndex);

	void UpdateDrawBatches(FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex);
	void UpdateIndirectBuffers(const FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex);
};
