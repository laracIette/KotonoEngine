#pragma once
#include "InterfaceUniformData.h"
#include "frames_in_flight.h"
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
	using ProxiesPool = KtPool<Proxy*>;
	using StagingProxiesMap = std::unordered_map<Proxy*, int32_t>;
	using DeleteProxiesMap = std::unordered_map<Proxy*, uint32_t>;

public:
	void Init();
	void Update(const uint32_t frameIndex);
	void Cleanup() const;

	void MarkCommandBuffersDirty();

	// todo: later if needed
	void SetUniformData(const KtInterfaceUniformData& uniformData);

	void RegisterProxy(Proxy* proxy);
	void UnregisterProxy(Proxy* proxy);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	Proxy* CreateProxy() const;
	// Stage deletion of the proxy after KT_FRAMES_IN_FLIGHT frames, 
	// warning: MUST be called after UnregisterProxy
	void DeleteProxy(Proxy* proxy);

private:
	struct FrameData
	{
		struct ObjectBufferData
		{
			ProxiesPool proxies;
			ProxiesPool sortedProxies;
			VkCommandBuffer commandBuffer;
			bool isDirty;
		};

		KtInterfaceUniformData uniformData;
		std::unordered_map<const KtShader*, uint32_t> instanceIndices;

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
	void CreateCommandBuffer(const uint32_t frameIndex);
	void RecordCommandBuffer(const uint32_t frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);

	void UpdateProxies(const uint32_t frameIndex);
	void UpdateStagingProxies(const uint32_t frameIndex);
	void UpdateDescriptorSets(const ProxiesPool& renderQueueData, const uint32_t frameIndex);

	void DeleteProxies();
	
	void SortProxies(ProxiesPool& proxies, const uint32_t frameIndex);

	void CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesPool& proxies, const uint32_t frameIndex);
};

