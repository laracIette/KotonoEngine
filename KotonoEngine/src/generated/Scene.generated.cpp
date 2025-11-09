#include "Scene.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneObject.h"

void KScene::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	size_t i{ 0 };
	for (const auto& v : sceneObjects_)
	{
		serialize(json["sceneObjects_"][i], v);
	}
}

void KScene::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	size_t i{ 0 };
	for (auto& v : sceneObjects_)
	{
		deserialize(json["sceneObjects_"][i], v);
	}
}
