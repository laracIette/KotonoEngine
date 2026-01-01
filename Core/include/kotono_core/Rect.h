#pragma once
#include <glm/vec2.hpp>
#include "Anchor.h"
struct URect final
{
	//glm::vec2 size{ 0.0f, 0.0f };
	glm::vec2 position{ 0.0f, 0.0f };
	glm::vec2 scale{ 1.0f, 1.0f };
	float rotation{ 0.0f };
	i32 layer{ 0 };
	EAnchor anchor{ EAnchor::Center };
};

