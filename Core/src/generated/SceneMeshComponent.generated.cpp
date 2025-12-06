#include "SceneMeshComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KSceneMeshComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["shader_"], shader_);
	serialize(json["model_"], model_);
}

void KSceneMeshComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("shader_"), shader_);
	deserialize(json.at("model_"), model_);
}
