#pragma once
#include <kotono_framework/glm_includes.h>
#include "Anchor.h"

struct URect final
{
	glm::vec2 size{ 0.0f, 0.0f };
	glm::vec2 position{ 0.0f, 0.0f };
	glm::vec2 scale{ 1.0f, 1.0f };
	float rotation{ 0.0f };
	EAnchor anchor{ EAnchor::Center };
};

