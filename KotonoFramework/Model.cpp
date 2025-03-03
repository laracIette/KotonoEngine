#include "Model.h"
#include "Framework.h"
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "log.h"

KtModel::KtModel(const std::filesystem::path& path) :
	_path(path)
{
	Load();
	CreateVertexBuffer();
	CreateIndexBuffer();
}

KtModel::~KtModel()
{
	KT_DEBUG_LOG("cleaning up model");
	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), _indexBuffer.Buffer, _indexBuffer.Allocation);
	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), _vertexBuffer.Buffer, _vertexBuffer.Allocation);
	KT_DEBUG_LOG("cleaned up model");
}

const std::filesystem::path& KtModel::GetPath() const
{
	return _path;
}

const std::vector<KtVertex>& KtModel::GetVertices() const
{
	return _vertices;
}

const std::vector<uint32_t>& KtModel::GetIndices() const
{
	return _indices;
}

const VkBuffer KtModel::GetVertexBuffer() const
{
	return _vertexBuffer.Buffer;
}

const VkBuffer KtModel::GetIndexBuffer() const
{
	return _indexBuffer.Buffer;
}

void KtModel::CmdBind(VkCommandBuffer commandBuffer) const
{
	VkBuffer vertexBuffers[] = { _vertexBuffer.Buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, _indexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void KtModel::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount) const
{
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(_indices.size()), instanceCount, 0, 0, 0);
}

void KtModel::Load()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(_path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || !scene->HasMeshes())
	{
		throw std::runtime_error("Failed to load model: " + _path.string());
	}

	std::unordered_map<KtVertex, uint32_t> uniqueVertices{};

	for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
	{
		const aiMesh* mesh = scene->mMeshes[m];

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace& face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				const aiVector3D pos = mesh->mVertices[face.mIndices[j]];
				const aiVector3D texCoord = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][face.mIndices[j]] : aiVector3D(0.0f, 0.0f, 0.0f);

				KtVertex vertex{};
				vertex.Position = { pos.x, pos.y, pos.z };
				vertex.TexCoord = { texCoord.x, texCoord.y };
				vertex.Color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(_vertices.size());
					_vertices.push_back(vertex);
				}

				_indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
}

void KtModel::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(_vertices[0]) * _vertices.size();

	KtAllocatedBuffer stagingBuffer;
	Framework.GetWindow().GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, _vertices.data(), (size_t)bufferSize);

	Framework.GetWindow().GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		_vertexBuffer
	);

	Framework.GetWindow().GetContext().CopyBuffer(stagingBuffer.Buffer, _vertexBuffer.Buffer, bufferSize);

	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
}

void KtModel::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(_indices[0]) * _indices.size();

	KtAllocatedBuffer stagingBuffer;
	Framework.GetWindow().GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, _indices.data(), (size_t)bufferSize);

	Framework.GetWindow().GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		_indexBuffer
	);

	Framework.GetWindow().GetContext().CopyBuffer(stagingBuffer.Buffer, _indexBuffer.Buffer, bufferSize);

	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
}
