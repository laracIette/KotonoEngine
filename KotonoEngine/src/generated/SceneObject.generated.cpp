#include "SceneObject.h"
#include "serialize.h"

void TSceneObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}
