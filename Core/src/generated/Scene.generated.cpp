#include "Scene.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneObject.h"

void KScene::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	json["sceneObjects_"] = nlohmann::json::array({});
	for (size_t i{ 0 }; i < sceneObjects_.size(); ++i)
	{
		serialize(json["sceneObjects_"][i], sceneObjects_[i]);
	}
}

void KScene::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	sceneObjects_.resize(json.at("sceneObjects_").size()); 
	for (size_t i{ 0 }; i < sceneObjects_.size(); ++i)
	{
		deserialize(json.at("sceneObjects_")[i], sceneObjects_[i]);
	}
}
