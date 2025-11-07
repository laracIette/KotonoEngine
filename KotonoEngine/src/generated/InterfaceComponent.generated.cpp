#include "InterfaceComponent.h"
#include "serialize.h"

void KInterfaceComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
