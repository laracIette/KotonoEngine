#pragma once
#include "frames_in_flight.h"
#include "SceneRenderable.h" 
#include "Vertex3D.h"
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>

class UModel final : public KtSceneRenderable
{
public:
	UModel(const UPath& path);
	~UModel();

	const UPath& Path() const;

	VkDeviceAddress GetVertexBufferAddress() const;
	VkBuffer GetIndexBuffer() const;
	u32 GetIndexCount() const;

	void CmdBind(VkCommandBuffer commandBuffer) const override;
	void CmdDraw(VkCommandBuffer commandBuffer, const u32 frameIndex) const override;
	
	void UpdateIndirectBuffer(const u32 firstInstance, const u32 instanceCount, const u32 frameIndex) const override;

private:
	void Load();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateIndirectBuffers();
	void CreateIndirectBuffer(const u32 frameIndex);
	void DestroyStagingVertexBuffer() const;
	void DestroyStagingIndexBuffer() const;

private:
	const UPath path_;

	std::vector<KtVertex3D> vertices_;
	std::vector<u32> indices_;
	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;
	KtFramesInFlightArray<KtAllocatedBuffer> indirectBuffers_;
	KtAllocatedBuffer stagingVertexBuffer_;
	KtAllocatedBuffer stagingIndexBuffer_;
};
