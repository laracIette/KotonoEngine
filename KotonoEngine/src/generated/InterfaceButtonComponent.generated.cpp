#include "InterfaceButtonComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KInterfaceButtonComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KInterfaceButtonComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
