#pragma once
#include <vk_mem_alloc.h>
#include <filesystem>
#include "Vertex.h"
struct KtModel
{
	const std::filesystem::path Path;
	std::vector<KtVertex> Vertices;
	std::vector<uint32_t> Indices; 
	VkBuffer VertexBuffer;
	VkBuffer IndexBuffer;
	VmaAllocation VertexBufferAllocation;
	VmaAllocation IndexBufferAllocation;

	KtModel(const std::filesystem::path& path);
	~KtModel();

private:
	void Load();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
};

