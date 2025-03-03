#pragma once
#include <vk_mem_alloc.h>
#include <filesystem>
#include "Vertex.h"
#include "AllocatedBuffer.h"
class KtModel final
{
public:
	KtModel(const std::filesystem::path& path);
	~KtModel();

	const std::filesystem::path& GetPath() const;
	const std::vector<KtVertex>& GetVertices() const;
	const std::vector<uint32_t>& GetIndices() const; 
	const VkBuffer GetVertexBuffer() const;
	const VkBuffer GetIndexBuffer() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;
	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount) const;

private:
	const std::filesystem::path _path;
	std::vector<KtVertex> _vertices;
	std::vector<uint32_t> _indices;
	KtAllocatedBuffer _vertexBuffer;
	KtAllocatedBuffer _indexBuffer;

	void Load();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
};

