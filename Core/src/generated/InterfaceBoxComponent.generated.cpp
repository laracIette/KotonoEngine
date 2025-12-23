#include "InterfaceBoxComponent.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KInterfaceBoxComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KInterfaceBoxComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}

UPtr<KInterfaceBoxComponent> KInterfaceBoxComponent::Ptr() const
{
	return static_cast<UPtrOwner<KInterfaceBoxComponent>*>(ptrOwner_);
}
