#include "Scene.h"
#include "serialize.h"

void KScene::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
