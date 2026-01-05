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

std::vector<UVariableInfo> KInterfaceBoxComponent::GetMemberVariables() const
{
	auto result{ Base::GetMemberVariables() };
	result.insert(result.end(), {

	});
	return result;
}

UPtr<KInterfaceBoxComponent> KInterfaceBoxComponent::Ptr() const
{
	return static_cast<UPtrOwner<KInterfaceBoxComponent>*>(ptrOwner_);
}
