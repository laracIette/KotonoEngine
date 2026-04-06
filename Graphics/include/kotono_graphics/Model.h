#pragma once
#include "frames_in_flight.h"
#include "SceneRenderable.h" 
#include "Vertex3D.h"
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <kotono_io/serialize_base.h>
class KtModel final : public KtSceneRenderable
{
public:
	KtModel(const UPath& path);

	void Init();
	void Cleanup();

	const UPath& Path() const;

	void CmdBind(VkCommandBuffer commandBuffer) const override;
	void CmdDraw(VkCommandBuffer commandBuffer, const u32 frameIndex) const override;
	
	void UpdateIndirectBuffer(const u32 firstInstance, const u32 instanceCount, const u32 frameIndex) const override;

private:
	void Load();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateIndirectBuffers();
	void CreateIndirectBuffer(const u32 frameIndex);
	void DestroyStagingVertexBuffer();
	void DestroyStagingIndexBuffer();

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

template <>
struct USerialize<KtModel>
{
	void operator()(nlohmann::json& json, const KtModel* v) const;
};

template <>
struct UDeserialize<KtModel>
{
	void operator()(const nlohmann::json& json, KtModel*& v) const;
};
