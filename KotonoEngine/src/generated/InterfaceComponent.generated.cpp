#include "InterfaceComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KInterfaceComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KInterfaceComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
