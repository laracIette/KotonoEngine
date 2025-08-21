#include "Model.h"
#include "Framework.h"
#include "Context.h"
#include <unordered_map>
#define ASSIMP_BUILD_NO_EXPORT
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <array>
#include "log.h"

KtModel::KtModel(const std::filesystem::path& path) :
	path_(path)
{
}

void KtModel::Init()
{
	Load();
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void KtModel::Cleanup() const
{
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_LOW, "cleaning up model");

	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), indexBuffer_.Buffer, indexBuffer_.Allocation);
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), vertexBuffer_.Buffer, vertexBuffer_.Allocation);
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_LOW, "cleaned up model");
}

const std::filesystem::path& KtModel::GetPath() const
{
	return path_;
}

void KtModel::CmdBind(VkCommandBuffer commandBuffer) const
{
	const std::array<VkBuffer, 1> vertexBuffers = { vertexBuffer_.Buffer };
	const std::array<VkDeviceSize, 1> offsets = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, static_cast<uint32_t>(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer_.Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void KtModel::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount, const uint32_t firstInstance) const
{
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices_.size()), instanceCount, 0, 0, firstInstance);
}

void KtModel::Load()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path_.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || !scene->HasMeshes())
	{
		throw std::runtime_error("Failed to load model: " + path_.string());
	}

	std::unordered_map<KtVertex3D, uint32_t> uniqueVertices{};

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

				KtVertex3D vertex{};
				vertex.Position = { pos.x, pos.y, pos.z };
				vertex.TexCoord = { texCoord.x, texCoord.y };
				vertex.Color = { 1.0f, 1.0f, 1.0f };

				if (!uniqueVertices.contains(vertex))
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices_.size());
					vertices_.push_back(vertex);
				}

				indices_.push_back(uniqueVertices[vertex]);
			}
		}
	}
}

void KtModel::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(KtVertex3D) * vertices_.size();

	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingVertexBuffer_
	);

	memcpy(stagingVertexBuffer_.AllocationInfo.pMappedData, vertices_.data(), static_cast<size_t>(bufferSize));

	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		vertexBuffer_
	);

	Framework.GetContext().CopyBuffer(stagingVertexBuffer_.Buffer, vertexBuffer_.Buffer, bufferSize);
	Framework.GetContext().GetEventExecuteSingleTimeCommands().AddListener(KtDelegate(this, &KtModel::DestroyStagingVertexBuffer));
}

void KtModel::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(uint32_t) * indices_.size();

	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingIndexBuffer_
	);

	memcpy(stagingIndexBuffer_.AllocationInfo.pMappedData, indices_.data(), static_cast<size_t>(bufferSize));

	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		indexBuffer_
	);

	Framework.GetContext().CopyBuffer(stagingIndexBuffer_.Buffer, indexBuffer_.Buffer, bufferSize);
	Framework.GetContext().GetEventExecuteSingleTimeCommands().AddListener(KtDelegate(this, &KtModel::DestroyStagingIndexBuffer));
}

void KtModel::DestroyStagingVertexBuffer()
{
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), stagingVertexBuffer_.Buffer, stagingVertexBuffer_.Allocation);
}

void KtModel::DestroyStagingIndexBuffer()
{
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), stagingIndexBuffer_.Buffer, stagingIndexBuffer_.Allocation);
}
