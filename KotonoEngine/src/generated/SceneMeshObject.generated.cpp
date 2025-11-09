#include "SceneMeshObject.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneMeshComponent.h"

void TSceneMeshObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["meshComponent_"], meshComponent_);
}

void TSceneMeshObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json["meshComponent_"], meshComponent_);
}
