#include "Timer.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void KTimer::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void KTimer::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}
