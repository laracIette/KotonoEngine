#include "InterfaceImageComponent.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>


void KInterfaceImageComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);

}

void KInterfaceImageComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);

}

std::vector<UVariableInfo> KInterfaceImageComponent::GetMemberVariables() const
{
	auto result{ Base::GetMemberVariables() };
	result.insert(result.end(), {

	});
	return result;
}

UPtr<KInterfaceImageComponent> KInterfaceImageComponent::Ptr() const
{
	return static_cast<UPtrOwner<KInterfaceImageComponent>*>(ptrOwner_);
}
