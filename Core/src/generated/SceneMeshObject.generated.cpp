#include "SceneMeshObject.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void TSceneMeshObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void TSceneMeshObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
