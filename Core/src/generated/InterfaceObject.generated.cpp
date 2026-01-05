#include "InterfaceObject.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceComponent.h"
#include "Interface.h"


void RInterfaceObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);

}

void RInterfaceObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);

}

std::vector<UVariableInfo> RInterfaceObject::GetMemberVariables() const
{
	auto result{ Base::GetMemberVariables() };
	result.insert(result.end(), {

	});
	return result;
}

UPtr<RInterfaceObject> RInterfaceObject::Ptr() const
{
	return static_cast<UPtrOwner<RInterfaceObject>*>(ptrOwner_);
}
