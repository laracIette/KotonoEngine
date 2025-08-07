#include "math_utils.h"
#include <cmath>

float round(const float num, const int digits) noexcept
{
    const float factor = static_cast<float>(std::pow(10.0f, digits));
    return std::round(num * factor) / factor;
}

double round(const double num, const int digits) noexcept
{
    const double factor = std::pow(10.0, digits);
    return std::round(num * factor) / factor;
}
