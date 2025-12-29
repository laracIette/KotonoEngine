#include "Model.h"
#include <kotono_platform/Context.h>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <array>
#include <kotono_common/log.h>

KtModel::KtModel(const std::filesystem::path& path) :
	path_(path)
{
}

void KtModel::Init()
{
	Load();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateIndirectBuffers();
}

void KtModel::Cleanup()
{
	vmaDestroyBuffer(Context.GetAllocator(), indexBuffer_.Buffer, indexBuffer_.Allocation);
	vmaDestroyBuffer(Context.GetAllocator(), vertexBuffer_.Buffer, vertexBuffer_.Allocation);
	for (auto& indirectBuffer : indirectBuffers_)
	{
		vmaDestroyBuffer(Context.GetAllocator(), indirectBuffer.Buffer, indirectBuffer.Allocation);
	}
	KT_LOG(ELogImportanceLevel::Low, "Graphics.KtModel::Cleanup()", "cleaned up %s", Path().string().c_str());
}

const std::filesystem::path& KtModel::Path() const
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

void KtModel::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex) const
{
	vkCmdDrawIndexedIndirect(commandBuffer, indirectBuffers_[frameIndex].Buffer, 0, 1, sizeof(VkDrawIndexedIndirectCommand));
}

void KtModel::UpdateIndirectBuffer(const uint32_t firstInstance, const uint32_t instanceCount, const uint32_t frameIndex) const
{
	auto* cmd{ static_cast<VkDrawIndexedIndirectCommand*>(indirectBuffers_[frameIndex].AllocationInfo.pMappedData) };
	cmd->instanceCount = instanceCount;
	cmd->firstInstance = firstInstance;
}

void KtModel::Load()
{
	Assimp::Importer importer{};
	const aiScene* scene = importer.ReadFile(path_.string().c_str(), 
		aiProcess_Triangulate | 
		aiProcess_FlipUVs |
		aiProcess_MakeLeftHanded |
		//aiProcess_FlipWindingOrder | 
		aiProcess_JoinIdenticalVertices
	);

	if (!scene || !scene->HasMeshes())
	{
		throw std::runtime_error("Failed to load model: " + path_.string());
	}

	std::unordered_map<KtVertex3D, uint32_t> uniqueVertices{};

	for (uint32_t m{ 0 }; m < scene->mNumMeshes; ++m)
	{
		const aiMesh* mesh{ scene->mMeshes[m] };

		for (uint32_t i{ 0 }; i < mesh->mNumFaces; ++i)
		{
			const aiFace& face{ mesh->mFaces[i] };

			for (uint32_t j{ 0 }; j < face.mNumIndices; ++j)
			{
				const aiVector3D pos{ mesh->mVertices[face.mIndices[j]] };
				const aiVector3D texCoord{ mesh->mTextureCoords[0] 
					? mesh->mTextureCoords[0][face.mIndices[j]] 
					: aiVector3D(0.0f, 0.0f, 0.0f) 
				};

				const KtVertex3D vertex{
					.position = { pos.x, pos.y, pos.z },
					.color = { 1.0f, 1.0f, 1.0f },
					.texCoord = { texCoord.x, texCoord.y },
				};

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
	const VkDeviceSize bufferSize{ sizeof(KtVertex3D) * vertices_.size() };

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingVertexBuffer_
	);

	memcpy(stagingVertexBuffer_.AllocationInfo.pMappedData, vertices_.data(), static_cast<size_t>(bufferSize));

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		vertexBuffer_
	);

	Context.CopyBuffer(stagingVertexBuffer_.Buffer, vertexBuffer_.Buffer, bufferSize);
	Context.GetEventExecuteSingleTimeCommands().AddListener(KtDelegate(this, &KtModel::DestroyStagingVertexBuffer));
}

void KtModel::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize{ sizeof(uint32_t) * indices_.size() };

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingIndexBuffer_
	);

	memcpy(stagingIndexBuffer_.AllocationInfo.pMappedData, indices_.data(), static_cast<size_t>(bufferSize));

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		indexBuffer_
	);

	Context.CopyBuffer(stagingIndexBuffer_.Buffer, indexBuffer_.Buffer, bufferSize);
	Context.GetEventExecuteSingleTimeCommands().AddListener(KtDelegate(this, &KtModel::DestroyStagingIndexBuffer));
}

void KtModel::CreateIndirectBuffers()
{
	for (size_t i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateIndirectBuffer(static_cast<uint32_t>(i));
	}
}

void KtModel::CreateIndirectBuffer(const uint32_t frameIndex)
{
	const VkDeviceSize bufferSize{ sizeof(VkDrawIndexedIndirectCommand) };

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		indirectBuffers_[frameIndex]
	);

	const VkDrawIndexedIndirectCommand cmd{
		.indexCount = static_cast<uint32_t>(indices_.size()),
		.instanceCount = 0,
		.firstIndex = 0,
		.vertexOffset = 0,
		.firstInstance = 0,
	};

	memcpy(indirectBuffers_[frameIndex].AllocationInfo.pMappedData, &cmd, sizeof(cmd));
}

void KtModel::DestroyStagingVertexBuffer()
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingVertexBuffer_.Buffer, stagingVertexBuffer_.Allocation);
}

void KtModel::DestroyStagingIndexBuffer()
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingIndexBuffer_.Buffer, stagingIndexBuffer_.Allocation);
}
