#include "SceneComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneObject.h"
#include "SceneObject.h"

void KSceneComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	json["children_"] = nlohmann::json::array({});
	for (size_t i{ 0 }; i < children_.size(); ++i)
	{
		serialize(json["children_"][i], children_[i]);
	}
	serialize(json["transform_"], transform_);
	serialize(json["visibility_"], visibility_);
	serialize(json["mobility_"], mobility_);
	serialize(json["componentIndex_"], componentIndex_);
	serialize(json["childrenIndex_"], childrenIndex_);
}

void KSceneComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	children_.resize(json.at("children_").size()); 
	for (size_t i{ 0 }; i < children_.size(); ++i)
	{
		deserialize(json.at("children_")[i], children_[i]);
	}
	deserialize(json.at("transform_"), transform_);
	deserialize(json.at("visibility_"), visibility_);
	deserialize(json.at("mobility_"), mobility_);
	deserialize(json.at("componentIndex_"), componentIndex_);
	deserialize(json.at("childrenIndex_"), childrenIndex_);
}
