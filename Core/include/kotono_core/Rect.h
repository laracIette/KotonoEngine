#pragma once
#include <glm/vec2.hpp>
#include "Anchor.h"
#include "serialize_glm.h"
struct URect final
{
	glm::vec2 position{ 0.0f, 0.0f };
	glm::vec2 scale{ 1.0f, 1.0f };
	float rotation{ 0.0f };
	i32 layer{ 0 };
	EAnchor anchor{ EAnchor::Center };
};

template <>
struct USerialize<URect>
{
	void operator()(nlohmann::json& json, const URect& v) const
	{
		USerialize<glm::vec2>{}(get(json, "position"), v.position);
		USerialize<glm::vec2>{}(get(json, "scale"), v.scale);
		USerialize<float>{}(get(json, "rotation"), v.rotation);
		USerialize<i32>{}(get(json, "layer"), v.layer);
		USerialize<EAnchor>{}(get(json, "anchor"), v.anchor);
	}
};

template <>
struct UDeserialize<URect>
{
	void operator()(const nlohmann::json& json, URect& v) const
	{
		UDeserialize<glm::vec2>{}(get(json, "position"), v.position);
		UDeserialize<glm::vec2>{}(get(json, "scale"), v.scale);
		UDeserialize<float>{}(get(json, "rotation"), v.rotation);
		UDeserialize<i32>{}(get(json, "layer"), v.layer);
		UDeserialize<EAnchor>{}(get(json, "anchor"), v.anchor);
	}
};
