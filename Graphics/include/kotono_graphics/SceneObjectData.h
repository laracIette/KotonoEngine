#pragma once
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
struct KtSceneObjectData final
{
	glm::mat4 modelMatrix{ glm::identity<glm::mat4>() };
	glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
};

