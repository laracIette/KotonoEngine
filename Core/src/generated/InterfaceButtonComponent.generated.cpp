#include "InterfaceButtonComponent.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceColliderComponent.h"

void KInterfaceButtonComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KInterfaceButtonComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}

UPtr<KInterfaceButtonComponent> KInterfaceButtonComponent::Ptr() const
{
	return static_cast<UPtrOwner<KInterfaceButtonComponent>*>(ptrOwner_);
}
