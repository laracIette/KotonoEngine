#include "utils.h"
#include <glm/vec2.hpp>
#include <kotono_platform/WindowViewport.h>

glm::vec2 px_to_ndc_size(const glm::vec2& px) noexcept
{
    return px / glm::vec2{ SWindowViewport::GetExtent() } * glm::vec2{ 1.0f, -1.0f } * 2.0f;
}

glm::vec2 px_to_ndc_pos(const glm::vec2& px) noexcept
{
    return px / glm::vec2{ SWindowViewport::GetExtent() } * 2.0f - 1.0f;
}
