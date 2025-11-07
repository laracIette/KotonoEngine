#include "InterfaceBoxComponent.h"
#include "serialize.h"

void KInterfaceBoxComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
