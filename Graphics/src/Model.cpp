#include "Model.h"
#include "IndexBuffer.h"
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
	firstIndex_ = IndexBuffer.RegisterIndices(indices_);
}

UModel::~UModel()
{
	vmaDestroyBuffer(Context.GetAllocator(), vertexBuffer_.buffer, vertexBuffer_.allocation);
	KT_LOG(ELogImportanceLevel::Low, "Graphics", "cleaned up {0}", Path().ToString());
}

const UPath& UModel::Path() const
{
	return path_;
}

VkDeviceAddress UModel::GetVertexBufferAddress() const
{
	return vertexBuffer_.bda;
}

u32 UModel::GetIndexCount() const
{
	return indices_.size();
}

u32 UModel::GetFirstIndex() const
{
	return firstIndex_;
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

	Context.CreateBuffer(stagingVertexBuffer_
		, bufferSize
		, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
	
	std::memcpy(stagingVertexBuffer_.allocationInfo.pMappedData, vertices_.data(), static_cast<size>(bufferSize));

	Context.CreateBuffer(vertexBuffer_
		, bufferSize
		, VK_BUFFER_USAGE_TRANSFER_DST_BIT
		| VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT
	);

	Context.CopyBuffer(stagingVertexBuffer_.buffer, vertexBuffer_.buffer, bufferSize);
	Context.GetEventExecuteSingleTimeCommands().AddListener(this, &UModel::DestroyStagingVertexBuffer);
}

void UModel::DestroyStagingVertexBuffer() const
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingVertexBuffer_.buffer, stagingVertexBuffer_.allocation);
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
