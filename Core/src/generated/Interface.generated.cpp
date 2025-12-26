#include "Interface.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceObject.h"

void KInterface::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	json["interfaceObjects_"] = nlohmann::json::array({});
	for (size_t i{ 0 }; i < interfaceObjects_.size(); ++i)
	{
		serialize(json["interfaceObjects_"][i], interfaceObjects_[i]);
	}
}

void KInterface::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	interfaceObjects_.resize(json.at("interfaceObjects_").size()); 
	for (size_t i{ 0 }; i < interfaceObjects_.size(); ++i)
	{
		deserialize(json.at("interfaceObjects_")[i], interfaceObjects_[i]);
	}
}

UPtr<KInterface> KInterface::Ptr() const
{
	return static_cast<UPtrOwner<KInterface>*>(ptrOwner_);
}
