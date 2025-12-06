#pragma once
#include "SceneRenderable.h" 
#include <filesystem>
#include "Vertex3D.h"
#include <kotono_platform/AllocatedBuffer.h>
class KtModel final : public KtSceneRenderable
{
public:
	KtModel(const std::filesystem::path& path);

	void Init();
	void Cleanup() const;

	const std::filesystem::path& Path() const;

	void CmdBind(VkCommandBuffer commandBuffer) const override;
	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount, const uint32_t firstInstance) const override;

private:
	const std::filesystem::path path_;

	std::vector<KtVertex3D> vertices_;
	std::vector<uint32_t> indices_;
	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;
	KtAllocatedBuffer stagingVertexBuffer_;
	KtAllocatedBuffer stagingIndexBuffer_;

	void Load();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void DestroyStagingVertexBuffer();
	void DestroyStagingIndexBuffer();
};

