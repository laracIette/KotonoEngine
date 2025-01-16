#include "Clamped.h"
#include <algorithm> 
#include <glm/glm.hpp> 
#include <glm/gtc/constants.hpp>

template<typename T>
Clamped<T>::Clamped(const T& min, const T& max) :
	_min(min),
	_max(max)
{
}

template<typename T>
const T Clamped<T>::Clamp(const T& value) const
{
    if constexpr (std::is_arithmetic_v<T>)
    {
        return std::min(_max, std::max(_min, value));
    }
    else
    {
        return glm::clamp(value, _min, _max);
    }
}
