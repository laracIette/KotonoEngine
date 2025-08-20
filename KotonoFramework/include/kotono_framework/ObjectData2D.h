#pragma once
#include "glm_includes.h"
struct KtObjectData2D final
{
	glm::mat4 modelMatrix{ glm::identity<glm::mat4>() };
	glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
};

