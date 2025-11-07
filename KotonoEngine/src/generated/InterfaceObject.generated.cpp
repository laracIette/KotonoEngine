#include "InterfaceObject.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void RInterfaceObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void RInterfaceObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
