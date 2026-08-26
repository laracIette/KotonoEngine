#include "Material.h"

#include <kotono_io/serialize_base.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

AMaterial::AMaterial(UPath const& path)
	: AAsset(path)
{
}

AMaterial::Data AMaterial::GetData() const
{
	nlohmann::json json{};
	SSerializer::Deserialize(json, GetPath());

	return{
		.albedo = UDeserialize<UPath>{}(json["albedo"]),
		.normal = UDeserialize<UPath>{}(json["normal"]),
		.orm = UDeserialize<UPath>{}(json["orm"]),
		.emissive = UDeserialize<UPath>{}(json["emissive"]),
		.materialType = UDeserialize<u32>{}(json["materialType"]),
		.sampler = UDeserialize<UPath>{}(json["sampler"]),
	};
}
