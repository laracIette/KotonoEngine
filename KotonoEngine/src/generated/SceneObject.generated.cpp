#include "SceneObject.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneComponent.h"
#include "Scene.h"

void TSceneObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void TSceneObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
