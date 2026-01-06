#include "Object.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>


void KObject::SerializeTo(nlohmann::json& json) const
{
	serialize(json["guid_"], guid_);
	serialize(json["type_"], type_);
	serialize(json["name_"], name_);

}

void KObject::DeserializeFrom(const nlohmann::json& json)
{
	deserialize(json.at("guid_"), guid_);
	deserialize(json.at("type_"), type_);
	deserialize(json.at("name_"), name_);

}

std::vector<UVariableInfo> KObject::GetMemberVariables() const
{
	return {
		{ "UGuid", "guid_", offsetof(Self, guid_) },
		{ "std::string", "type_", offsetof(Self, type_) },
		{ "std::string", "name_", offsetof(Self, name_) },

	};
}

UPtr<KObject> KObject::Ptr() const
{
	return static_cast<UPtrOwner<KObject>*>(ptrOwner_);
}
