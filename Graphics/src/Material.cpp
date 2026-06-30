#include "Material.h"
#include <kotono_common/AssetManager.h>
#include "MaterialBuffer.h"
#include "Sampler.h"
#include "Texture.h"
#include <kotono_io/Serializer.h>
#include <kotono_io/serialize_base.h>
#include <nlohmann/json.hpp>

UMaterial::UMaterial(const UPath& path)
	: path_{ path }
	, index_{}
{
	nlohmann::json json{};
	USerializer::Deserialize(json, path);

	std::string albedo;
	UDeserialize<std::string>{}(json["albedo"], albedo);
	std::string normal;
	UDeserialize<std::string>{}(json["normal"], normal);
	std::string roughness;
	UDeserialize<std::string>{}(json["roughness"], roughness);
	std::string emissive;
	UDeserialize<std::string>{}(json["emissive"], emissive);
	u32 materialType;
	UDeserialize<u32>{}(json["materialType"], materialType);
	UPath sampler;
	UDeserialize<UPath>{}(json["sampler"], sampler);

	index_ = MaterialBuffer.RegisterMaterial({
		.albedoIndex = UAssetManager<UTexture>::Get(albedo)->GetIndex(),
		.normalIndex = UAssetManager<UTexture>::Get(normal)->GetIndex(),
		.roughnessIndex = UAssetManager<UTexture>::Get(roughness)->GetIndex(),
		.emissiveIndex = UAssetManager<UTexture>::Get(emissive)->GetIndex(),
		.materialType = materialType,
		.samplerIndex = UAssetManager<USampler>::Get(sampler)->GetIndex(),
	});
}

u32 UMaterial::GetIndex() const
{
	return index_;
}
