#include "Model.h"
#include "Framework.h"
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

KtModel::KtModel(const std::filesystem::path& path) :
	Path(path)
{
	Load();
	CreateVertexBuffer();
	CreateIndexBuffer();
}

KtModel::~KtModel()
{
	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), IndexBuffer, IndexBufferAllocation);
	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), VertexBuffer, VertexBufferAllocation);
}

void KtModel::Load()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(Path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || !scene->HasMeshes())
	{
		throw std::runtime_error("Failed to load model: " + Path.string());
	}

	std::unordered_map<KtVertex, uint32_t> uniqueVertices{};

	for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
	{
		 aiMesh* mesh = scene->mMeshes[m];

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			 aiFace& face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				 aiVector3D pos = mesh->mVertices[face.mIndices[j]];
				 aiVector3D texCoord = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][face.mIndices[j]] : aiVector3D(0.0f, 0.0f, 0.0f);

				KtVertex vertex{};
				vertex.Position = { pos.x, pos.y, pos.z };
				vertex.TexCoord = { texCoord.x, texCoord.y };
				vertex.Color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(Vertices.size());
					Vertices.push_back(vertex);
				}

				Indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
}

void KtModel::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(Vertices[0]) * Vertices.size();

	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;
	VmaAllocationInfo stagingBufferAllocInfo;
	Framework.GetWindow().GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer,
		stagingBufferAllocation,
		stagingBufferAllocInfo
	);

	memcpy(stagingBufferAllocInfo.pMappedData, Vertices.data(), (size_t)bufferSize);

	VmaAllocationInfo vertexBufferAllocInfo;
	Framework.GetWindow().GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		VertexBuffer,
		VertexBufferAllocation,
		vertexBufferAllocInfo
	);

	Framework.GetWindow().GetContext().CopyBuffer(stagingBuffer, VertexBuffer, bufferSize);

	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), stagingBuffer, stagingBufferAllocation);
}

void KtModel::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(Indices[0]) * Indices.size();

	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;
	VmaAllocationInfo stagingBufferAllocInfo;
	Framework.GetWindow().GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer,
		stagingBufferAllocation,
		stagingBufferAllocInfo
	);

	memcpy(stagingBufferAllocInfo.pMappedData, Indices.data(), (size_t)bufferSize);

	VmaAllocationInfo indexBufferAllocInfo;
	Framework.GetWindow().GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		IndexBuffer,
		IndexBufferAllocation,
		indexBufferAllocInfo
	);

	Framework.GetWindow().GetContext().CopyBuffer(stagingBuffer, IndexBuffer, bufferSize);

	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), stagingBuffer, stagingBufferAllocation);
}
