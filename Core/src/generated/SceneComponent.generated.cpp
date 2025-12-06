#include "SceneComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneObject.h"
#include "SceneObject.h"

void KSceneComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["transform_"], transform_);
	serialize(json["visibility_"], visibility_);
	serialize(json["mobility_"], mobility_);
}

void KSceneComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("transform_"), transform_);
	deserialize(json.at("visibility_"), visibility_);
	deserialize(json.at("mobility_"), mobility_);
}
