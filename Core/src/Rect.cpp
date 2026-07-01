#include "Rect.h"
#include "serialize_glm.h"

void USerialize<URect>::operator()(nlohmann::json& json, const URect& v) const
{
	USerialize<glm::vec2>{}(get(json, "position"), v.position);
	USerialize<glm::vec2>{}(get(json, "scale"), v.scale);
	USerialize<f32>{}(get(json, "rotation"), v.rotation);
	USerialize<i32>{}(get(json, "layer"), v.layer);
	USerialize<EAnchor>{}(get(json, "anchor"), v.anchor);
}

void UDeserialize<URect>::operator()(const nlohmann::json& json, URect& v) const
{
	UDeserialize<glm::vec2>{}(get(json, "position"), v.position);
	UDeserialize<glm::vec2>{}(get(json, "scale"), v.scale);
	UDeserialize<f32>{}(get(json, "rotation"), v.rotation);
	UDeserialize<i32>{}(get(json, "layer"), v.layer);
	UDeserialize<EAnchor>{}(get(json, "anchor"), v.anchor);
}
