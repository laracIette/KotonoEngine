#include "InterfaceImageObject.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceImageComponent.h"

void RInterfaceImageObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void RInterfaceImageObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
