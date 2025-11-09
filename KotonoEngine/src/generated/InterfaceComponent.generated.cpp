#include "InterfaceComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceObject.h"
#include "InterfaceObject.h"

void KInterfaceComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KInterfaceComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
