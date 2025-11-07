#include "Task.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KTask::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KTask::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
