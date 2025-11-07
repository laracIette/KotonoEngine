#include "InterfaceImageObject.h"
#include "serialize.h"

void RInterfaceImageObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
