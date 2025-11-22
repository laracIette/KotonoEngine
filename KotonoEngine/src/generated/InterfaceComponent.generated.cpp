#include "InterfaceComponent.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "InterfaceObject.h"
#include "InterfaceObject.h"

void KInterfaceComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["rect_"], rect_);
	serialize(json["visibility_"], visibility_);
	serialize(json["layer_"], layer_);
	serialize(json["color_"], color_);
}

void KInterfaceComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("rect_"), rect_);
	deserialize(json.at("visibility_"), visibility_);
	deserialize(json.at("layer_"), layer_);
	deserialize(json.at("color_"), color_);
}
