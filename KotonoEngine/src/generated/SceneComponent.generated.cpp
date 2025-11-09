#include "SceneComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneObject.h"
#include "SceneObject.h"

void KSceneComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KSceneComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
