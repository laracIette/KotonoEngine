#include "SceneComponent.h"
#include "serialize.h"

void KSceneComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
