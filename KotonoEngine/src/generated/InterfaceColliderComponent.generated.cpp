#include "InterfaceColliderComponent.h"
#include "serialize.h"

void KInterfaceColliderComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
