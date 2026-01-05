#include "InterfaceComponent.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceObject.h"
#include "InterfaceObject.h"


void KInterfaceComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["rect_"], rect_);
	serialize(json["visibility_"], visibility_);
	serialize(json["color_"], color_);

}

void KInterfaceComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("rect_"), rect_);
	deserialize(json.at("visibility_"), visibility_);
	deserialize(json.at("color_"), color_);

}

std::vector<UVariableInfo> KInterfaceComponent::GetMemberVariables() const
{
	auto result{ Base::GetMemberVariables() };
	result.insert(result.end(), {
		{ "URect", offsetof(Self, rect_) },
		{ "EVisibility", offsetof(Self, visibility_) },
		{ "UColor", offsetof(Self, color_) },

	});
	return result;
}

UPtr<KInterfaceComponent> KInterfaceComponent::Ptr() const
{
	return static_cast<UPtrOwner<KInterfaceComponent>*>(ptrOwner_);
}
