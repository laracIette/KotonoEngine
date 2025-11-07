#include "SceneMeshComponent.h"
#include "serialize.h"

void KSceneMeshComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
