#include "Task.h"
#include "serialize.h"

void KTask::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
