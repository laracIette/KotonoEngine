#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <kotono_common/hash_utils.h>
#include <kotono_common/log.h>
#include <kotono_platform/Context.h>
#include <unordered_map>

UModel::UModel(const UPath& path) 
	: path_(path)
{
	Load();
	CreateVertexBuffer();
	CreateIndexBuffer();
}

UModel::~UModel()
{
	vmaDestroyBuffer(Context.GetAllocator(), indexBuffer_.Buffer, indexBuffer_.Allocation);
	vmaDestroyBuffer(Context.GetAllocator(), vertexBuffer_.Buffer, vertexBuffer_.Allocation);
	KT_LOG(ELogImportanceLevel::Low, "Graphics", "cleaned up {0}", Path().ToString());
}

const UPath& UModel::Path() const
{
	return path_;
}

VkDeviceAddress UModel::GetVertexBufferAddress() const
{
	const VkBufferDeviceAddressInfo addrInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
		.buffer = vertexBuffer_.Buffer,
	};
	return vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
}

VkBuffer UModel::GetIndexBuffer() const
{
	return indexBuffer_.Buffer;
}

u32 UModel::GetIndexCount() const
{
	return indices_.size();
}

void UModel::Load()
{
	Assimp::Importer importer{};
	const aiScene* scene{ importer.ReadFile(path_.ToPath().string().c_str()
		, aiProcess_Triangulate
		| aiProcess_FlipUVs
		| aiProcess_MakeLeftHanded
		| aiProcess_JoinIdenticalVertices
		| aiProcess_CalcTangentSpace
		| aiProcess_GenSmoothNormals
	) };

	if (!scene || !scene->HasMeshes())
	{
		throw std::runtime_error("Failed to load model: " + path_.ToString());
	}

	std::unordered_map<UVertex, u32> uniqueVertices{};

	for (u32 m{ 0 }; m < scene->mNumMeshes; ++m)
	{
		const aiMesh* mesh{ scene->mMeshes[m] };

		for (u32 i{ 0 }; i < mesh->mNumFaces; ++i)
		{
			const aiFace& face{ mesh->mFaces[i] };

			for (u32 j{ 0 }; j < face.mNumIndices; ++j)
			{
				const uint32_t idx{ face.mIndices[j] };

				const aiVector3D pos{ mesh->mVertices[idx] };

				const aiVector3D norm{ mesh->mNormals
					? mesh->mNormals[idx]
					: aiVector3D{ 0.0f, 1.0f, 0.0f } };

				const aiVector3D uv{ mesh->mTextureCoords[0]
					? mesh->mTextureCoords[0][idx]
					: aiVector3D{ 0.0f, 0.0f, 0.0f } };

				const aiVector3D tan{ mesh->mTangents
					? mesh->mTangents[idx]
					: aiVector3D{ 1.0f, 0.0f, 0.0f } };

				const aiVector3D bitan{ mesh->mBitangents
					? mesh->mBitangents[idx]
					: aiVector3D{ 0.0f, 0.0f, 1.0f } };

				const glm::vec3 N{ norm.x,  norm.y,  norm.z };
				const glm::vec3 T{ tan.x,   tan.y,   tan.z };
				const glm::vec3 B{ bitan.x, bitan.y, bitan.z };

				const float handedness{ glm::dot(glm::cross(N, T), B) < 0.0f ? -1.0f : 1.0f };

				const UVertex vertex{ 
					.position = { pos.x, pos.y, pos.z },
					.normal = N,
					.uv = { uv.x, uv.y },
					.tangent = { T, handedness },
				};

				if (!uniqueVertices.contains(vertex))
				{
					uniqueVertices[vertex] = static_cast<u32>(vertices_.size());
					vertices_.push_back(vertex);
				}

				indices_.push_back(uniqueVertices[vertex]);
			}
		}
	}
}

void UModel::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize{ sizeof(UVertex) * vertices_.size() };

	Context.CreateBuffer(bufferSize
		, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT 
		| VMA_ALLOCATION_CREATE_MAPPED_BIT
		, stagingVertexBuffer_
	);

	std::memcpy(stagingVertexBuffer_.AllocationInfo.pMappedData, vertices_.data(), static_cast<size>(bufferSize));

	Context.CreateBuffer(bufferSize
		, VK_BUFFER_USAGE_TRANSFER_DST_BIT 
		| VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT
		, vertexBuffer_
	);

	Context.CopyBuffer(stagingVertexBuffer_.Buffer, vertexBuffer_.Buffer, bufferSize);
	Context.GetEventExecuteSingleTimeCommands().AddListener(this, &UModel::DestroyStagingVertexBuffer);
}

void UModel::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize{ sizeof(u32) * indices_.size() };

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingIndexBuffer_
	);

	std::memcpy(stagingIndexBuffer_.AllocationInfo.pMappedData, indices_.data(), static_cast<size>(bufferSize));

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		indexBuffer_
	);

	Context.CopyBuffer(stagingIndexBuffer_.Buffer, indexBuffer_.Buffer, bufferSize);
	Context.GetEventExecuteSingleTimeCommands().AddListener(this, &UModel::DestroyStagingIndexBuffer);
}

void UModel::DestroyStagingVertexBuffer() const
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingVertexBuffer_.Buffer, stagingVertexBuffer_.Allocation);
}

void UModel::DestroyStagingIndexBuffer() const
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingIndexBuffer_.Buffer, stagingIndexBuffer_.Allocation);
}

bool UVertex::operator==(const UVertex& other) const noexcept
{
	return position == other.position
		&& normal == other.normal
		&& uv == other.uv
		&& tangent == other.tangent;
}

size std::hash<UVertex>::operator()(const UVertex& v) const noexcept
{
	::size h{};
	h = combine(h, std::hash<glm::vec3>{}(v.position));
	h = combine(h, std::hash<glm::vec3>{}(v.normal));
	h = combine(h, std::hash<glm::vec2>{}(v.uv));
	h = combine(h, std::hash<glm::vec2>{}(v.tangent));
	return h;
}
