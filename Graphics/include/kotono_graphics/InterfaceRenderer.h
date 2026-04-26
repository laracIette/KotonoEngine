#pragma once
#include "InterfaceUniformData.h"
#include "frames_in_flight.h"
#include "Scissor.h"
#include <kotono_platform/AllocatedBuffer.h>
#include <kotono_common/Pool.h>
#include <vulkan/vulkan_core.h>
#include <unordered_map>
class KtShader;
class UInterfaceProxy;
class KtInterfaceRenderer final
{
private:
	using Proxy = UInterfaceProxy;
	using ProxiesPool = UPool<Proxy*>;
	using StagingProxiesMap = std::unordered_map<Proxy*, i32>;
	using DeleteProxiesMap = std::unordered_map<Proxy*, u32>;

public:
	void Init();
	void Update(const u32 frameIndex);
	void Cleanup() const;

	void MarkCommandBuffersDirty();

	// todo: later if needed
	void SetUniformData(const KtInterfaceUniformData& uniformData);

	void RegisterProxy(Proxy* proxy);
	void UnregisterProxy(Proxy* proxy);

	void CmdDraw(VkCommandBuffer commandBuffer, const u32 frameIndex);

	Proxy* CreateProxy() const;
	// Stage deletion of the proxy after KT_FRAMES_IN_FLIGHT frames, 
	// warning: MUST be called after UnregisterProxy
	void DeleteProxy(Proxy* proxy);

private:
	struct DrawBatch
	{
		KtShader* shader;
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

		KtInterfaceUniformData uniformData;
		std::unordered_map<const KtShader*, u32> instanceIndices;

		ObjectBufferData objectBuffer;
	};

	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;
	KtAllocatedBuffer stagingVertexBuffer_;
	KtAllocatedBuffer stagingIndexBuffer_;

	// Those are only accessed from the game thread
	StagingProxiesMap stagingProxies_;
	DeleteProxiesMap deleteProxies_;

	// Those are updated by the game thread then accessed from the render thread
	KtFramesInFlightArray<FrameData> frameDatas_;

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void DestroyStagingVertexBuffer() const;
	void DestroyStagingIndexBuffer() const;

	void CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const;
	void CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const;

	void CreateCommandBuffers();
	void CreateCommandBuffer(const u32 frameIndex);
	void RecordCommandBuffer(const u32 frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const u32 frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);

	void UpdateProxies(const u32 frameIndex);
	void UpdateStagingProxies(const u32 frameIndex);
	void UpdateDescriptorSets(const ProxiesPool& renderQueueData, const u32 frameIndex);

	void DeleteProxies();
	
	void SortProxies(ProxiesPool& proxies, const u32 frameIndex);

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const std::vector<DrawBatch>& drawBatches, const u32 frameIndex);
	void UpdateDrawBatches(FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex);
};

