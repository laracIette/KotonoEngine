#include "Interface.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceImageObject.h"

void KInterface::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KInterface::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
