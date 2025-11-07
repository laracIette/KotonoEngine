#include "InterfaceObject.h"
#include "serialize.h"

void RInterfaceObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
