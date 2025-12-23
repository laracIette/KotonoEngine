#include "SceneObject.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneComponent.h"
#include "Scene.h"

void TSceneObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["rootComponent_"], rootComponent_);
	json["sceneComponents_"] = nlohmann::json::array({});
	for (size_t i{ 0 }; i < sceneComponents_.size(); ++i)
	{
		serialize(json["sceneComponents_"][i], sceneComponents_[i]);
	}
	json["children_"] = nlohmann::json::array({});
	for (size_t i{ 0 }; i < children_.size(); ++i)
	{
		serialize(json["children_"][i], children_[i]);
	}
	serialize(json["childrenIndex_"], childrenIndex_);
}

void TSceneObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("rootComponent_"), rootComponent_);
	sceneComponents_.resize(json.at("sceneComponents_").size()); 
	for (size_t i{ 0 }; i < sceneComponents_.size(); ++i)
	{
		deserialize(json.at("sceneComponents_")[i], sceneComponents_[i]);
	}
	children_.resize(json.at("children_").size()); 
	for (size_t i{ 0 }; i < children_.size(); ++i)
	{
		deserialize(json.at("children_")[i], children_[i]);
	}
	deserialize(json.at("childrenIndex_"), childrenIndex_);
}
