#include "Timer.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KTimer::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["isRepeat_"], isRepeat_);
	serialize(json["duration_"], duration_);
}

void KTimer::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("isRepeat_"), isRepeat_);
	deserialize(json.at("duration_"), duration_);
}
