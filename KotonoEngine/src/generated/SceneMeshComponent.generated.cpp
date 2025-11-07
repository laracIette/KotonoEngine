#include "SceneMeshComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KSceneMeshComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KSceneMeshComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
