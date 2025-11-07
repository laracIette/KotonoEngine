#include "InterfaceImageComponent.h"
#include "serialize.h"

void KInterfaceImageComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
