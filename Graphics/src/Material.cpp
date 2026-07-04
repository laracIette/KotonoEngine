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
	SSerializer::Deserialize(json, path);

	std::string albedo;
	UDeserialize<std::string>{}(json["albedo"], albedo);
	std::string normal;
	UDeserialize<std::string>{}(json["normal"], normal);
	std::string orm;
	UDeserialize<std::string>{}(json["orm"], orm);
	std::string emissive;
	UDeserialize<std::string>{}(json["emissive"], emissive);
	u32 materialType;
	UDeserialize<u32>{}(json["materialType"], materialType);
	UPath sampler;
	UDeserialize<UPath>{}(json["sampler"], sampler);

	index_ = MaterialBuffer.RegisterMaterial({
		.albedoIndex = SAssetManager<UTexture>::Get(albedo)->GetIndex(),
		.normalIndex = SAssetManager<UTexture>::Get(normal)->GetIndex(),
		.ormIndex = SAssetManager<UTexture>::Get(orm)->GetIndex(),
		.emissiveIndex = SAssetManager<UTexture>::Get(emissive)->GetIndex(),
		.materialType = materialType,
		.samplerIndex = SAssetManager<USampler>::Get(sampler)->GetIndex(),
	});
}

u32 UMaterial::GetIndex() const
{
	return index_;
}
