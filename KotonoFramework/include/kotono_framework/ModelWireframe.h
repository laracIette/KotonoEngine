#pragma once
#include <vma/vk_mem_alloc.h>
#include <filesystem>
#include "Vertex3D.h"
#include "AllocatedBuffer.h"
class KtModelWireframe final
{
public:
	KtModelWireframe(const std::filesystem::path& path);

	void Init();
	void Cleanup() const;

	const std::filesystem::path& GetPath() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;
	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount, const uint32_t firstInstance) const;

private:
	const std::filesystem::path _path;
	std::vector<glm::vec3> _vertices;
	std::vector<uint32_t> _indices;
	KtAllocatedBuffer _vertexBuffer;
	KtAllocatedBuffer _indexBuffer;

	void Load();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
};

