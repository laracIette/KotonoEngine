#include "InterfaceButtonComponent.h"
#include "serialize.h"

void KInterfaceButtonComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
