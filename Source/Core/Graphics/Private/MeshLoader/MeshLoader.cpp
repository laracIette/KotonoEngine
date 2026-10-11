#include "MeshLoader/MeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <check.h>

UMap<UPath, UMeshData> SMeshLoader::meshDatas_{};

auto SMeshLoader::Load(UPath const& path) -> UMeshData
{
	auto const it{ meshDatas_.Find(path) };
	if (meshDatas_.IsValidIterator(it))
	{
		return it->second;
	}
	
	UMeshData meshData{};
	
	Assimp::Importer importer{};
	aiScene const* scene{ importer.ReadFile(path.ToPath().string().c_str()
		, aiProcess_Triangulate
		| aiProcess_FlipUVs
		| aiProcess_MakeLeftHanded
		| aiProcess_JoinIdenticalVertices
		| aiProcess_CalcTangentSpace
		| aiProcess_GenSmoothNormals
	) };

	Check(Throw, scene && scene->HasMeshes(), "failed to load model: {0}", path.ToString());

	UMap<UVertex, u32> uniqueVertices{};

	for (u32 m{ 0 }; m < scene->mNumMeshes; ++m)
	{
		aiMesh const* mesh{ scene->mMeshes[m] };

		for (u32 i{ 0 }; i < mesh->mNumFaces; ++i)
		{
			aiFace const& face{ mesh->mFaces[i] };

			for (u32 j{ 0 }; j < face.mNumIndices; ++j)
			{
				uint32_t const idx{ face.mIndices[j] };

				aiVector3D const pos{ mesh->mVertices[idx] };

				aiVector3D const norm{ mesh->mNormals
					? mesh->mNormals[idx]
					: aiVector3D{ 0.0f, 1.0f, 0.0f } };

				aiVector3D const uv{ mesh->mTextureCoords[0]
					? mesh->mTextureCoords[0][idx]
					: aiVector3D{ 0.0f, 0.0f, 0.0f } };

				aiVector3D const tan{ mesh->mTangents
					? mesh->mTangents[idx]
					: aiVector3D{ 1.0f, 0.0f, 0.0f } };

				aiVector3D const bitan{ mesh->mBitangents
					? mesh->mBitangents[idx]
					: aiVector3D{ 0.0f, 0.0f, 1.0f } };

				glm::vec3 const N{ norm.x,  norm.y,  norm.z };
				glm::vec3 const T{ tan.x,   tan.y,   tan.z };
				glm::vec3 const B{ bitan.x, bitan.y, bitan.z };

				f32 const handedness{ glm::dot(glm::cross(N, T), B) < 0.0f ? -1.0f : 1.0f };

				UVertex const vertex{ 
					.position = { pos.x, pos.y, pos.z },
					.normal = N,
					.uv = { uv.x, uv.y },
					.tangent = { T, handedness },
				};
				
				auto const [it, emplaced] { uniqueVertices.TryEmplace(vertex, static_cast<u32>(meshData.vertices.size())) };
				if (emplaced)
				{
					uniqueVertices[vertex] = static_cast<u32>(meshData.vertices.size());
					meshData.vertices.push_back(vertex);
				}

				meshData.indices.push_back(uniqueVertices.At(vertex));
			}
		}
	}
	
	meshDatas_.TryEmplace(path, meshData);
	
	return meshData;
}
