#pragma once
#include "Renderable3D.h"
#include "Vertex3D.h"
#include "AllocatedBuffer.h"
#include <span>
#include <cstdint>
class KtModelWireframe final : public KtRenderable3D
{
public:
	void Init(const std::span<KtVertex3D> vertices, const std::span<uint32_t> indices);
	void Cleanup() const;

	const bool GetIsInit() const;

	void CmdBind(VkCommandBuffer commandBuffer) const override;
	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount, const uint32_t firstInstance) const override;

private:
	std::vector<glm::vec3> _vertices;
	std::vector<uint32_t> _indices;
	KtAllocatedBuffer _vertexBuffer;
	KtAllocatedBuffer _indexBuffer;

	bool _isInit;

	void CreateVertexBuffer();
	void CreateIndexBuffer();
};

