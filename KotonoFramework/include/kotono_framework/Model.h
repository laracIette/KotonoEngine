#pragma once
#include "Renderable3D.h" 
#include <filesystem>
#include "Vertex3D.h"
#include "AllocatedBuffer.h"
class KtModel final : public KtRenderable3D
{
public:
	KtModel(const std::filesystem::path& path);

	void Init();
	void Cleanup() const;

	const std::filesystem::path& GetPath() const;

	void CmdBind(VkCommandBuffer commandBuffer) const override;
	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount, const uint32_t firstInstance) const override;

private:
	const std::filesystem::path path_;

	std::vector<KtVertex3D> vertices_;
	std::vector<uint32_t> indices_;
	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;

	void Load();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
};

