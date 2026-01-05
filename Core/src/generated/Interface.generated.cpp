#include "Interface.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceObject.h"


void KInterface::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["interfaceObjects_"], interfaceObjects_);

}

void KInterface::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("interfaceObjects_"), interfaceObjects_);

}

std::vector<UVariableInfo> KInterface::GetMemberVariables() const
{
	auto result{ Base::GetMemberVariables() };
	result.insert(result.end(), {
		{ "KtPool<UPtr<RInterfaceObject>>", offsetof(Self, interfaceObjects_) },

	});
	return result;
}

UPtr<KInterface> KInterface::Ptr() const
{
	return static_cast<UPtrOwner<KInterface>*>(ptrOwner_);
}
