#include "Interface.h"
#include "serialize.h"

void KInterface::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
