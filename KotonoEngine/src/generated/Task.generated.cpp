#include "Task.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KTask::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["duration_"], duration_);
}

void KTask::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("duration_"), duration_);
}
