#include "Object.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KObject::SerializeTo(nlohmann::json& json) const
{
	serialize(json["guid_"], guid_);
	serialize(json["name_"], name_);
}

void KObject::DeserializeFrom(const nlohmann::json& json)
{
	deserialize(json.at("guid_"), guid_);
	deserialize(json.at("name_"), name_);
}
