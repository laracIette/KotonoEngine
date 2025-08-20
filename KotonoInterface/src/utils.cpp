#include "utils.h"
#include <kotono_framework/Viewport.h>

glm::vec2 px_to_ndc_size(const glm::vec2 px) noexcept
{
    return px / glm::vec2(WindowViewport.GetExtent()) * 2.0f;
}

glm::vec2 ndc_to_px_size(const glm::vec2 ndc) noexcept
{
    return ndc * glm::vec2(WindowViewport.GetExtent()) / 2.0f;
}

glm::vec2 px_to_ndc_pos(const glm::vec2 px) noexcept
{
    return px / glm::vec2(WindowViewport.GetExtent()) * 2.0f - 1.0f;
}

glm::vec2 ndc_to_px_pos(const glm::vec2 ndc) noexcept
{
    return (ndc + 1.0f) * glm::vec2(WindowViewport.GetExtent()) / 2.0f;
}