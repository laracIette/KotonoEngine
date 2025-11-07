#include "Timer.h"
#include "serialize.h"

void KTimer::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
