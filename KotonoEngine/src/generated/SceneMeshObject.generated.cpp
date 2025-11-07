#include "SceneMeshObject.h"
#include "serialize.h"

void TSceneMeshObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
