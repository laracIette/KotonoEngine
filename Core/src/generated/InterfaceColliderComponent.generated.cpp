#include "InterfaceColliderComponent.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KInterfaceColliderComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KInterfaceColliderComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}

UPtr<KInterfaceColliderComponent> KInterfaceColliderComponent::Ptr() const
{
	return static_cast<UPtrOwner<KInterfaceColliderComponent>*>(ptrOwner_);
}
