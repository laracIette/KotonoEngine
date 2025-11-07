#include "Scene.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KScene::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KScene::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
