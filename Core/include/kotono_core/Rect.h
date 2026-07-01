#pragma once
#include "Anchor.h"
#include <glm/vec2.hpp>
#include <kotono_io/serialize_base.h>
struct URect final
{
	glm::vec2 position{ 0.0f, 0.0f };
	glm::vec2 scale{ 1.0f, 1.0f };
	f32 rotation{ 0.0f };
	i32 layer{ 0 };
	EAnchor anchor{ EAnchor::Center };
};

template<>
struct USerialize<URect>
{
	void operator()(nlohmann::json& json, const URect& v) const;
};

template<>
struct UDeserialize<URect>
{
	void operator()(const nlohmann::json& json, URect& v) const;
};
