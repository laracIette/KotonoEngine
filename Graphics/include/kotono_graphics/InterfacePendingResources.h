#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
struct UPendingTexture final
{
	UPath path;
	EHandle handle;
};
struct UPendingSceneRender final
{
	EHandle handle;
	glm::uvec2 extent;
};