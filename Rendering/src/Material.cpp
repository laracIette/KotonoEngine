#include "Material.h"

#include "MaterialBuffer.h"
#include "Sampler.h"
#include "Texture.h"
#include <kotono_common/AssetManager.h>
#include <kotono_io/serialize_base.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

AMaterial::AMaterial(const UPath& path)
	: AAsset(path)
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
		.albedoIndex = SAssetManager<ATexture>::Get(albedo)->GetIndex(),
		.normalIndex = SAssetManager<ATexture>::Get(normal)->GetIndex(),
		.ormIndex = SAssetManager<ATexture>::Get(orm)->GetIndex(),
		.emissiveIndex = SAssetManager<ATexture>::Get(emissive)->GetIndex(),
		.materialType = materialType,
		.samplerIndex = SAssetManager<ASampler>::Get(sampler)->GetIndex(),
	});
}

u32 AMaterial::GetIndex() const
{
	return index_;
}
