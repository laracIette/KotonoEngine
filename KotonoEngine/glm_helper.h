#pragma once
#include <sstream>

constexpr const char* letters = "xyzw";

namespace glm
{
    template <typename T>
    const std::string to_string(const T& vec)
    {
        std::ostringstream oss;
        for (int i = 0; i < vec.length(); ++i)
        {
            oss << letters[i] << ": " << vec[i];
            if (i < vec.length() - 1)
                oss << ", ";
        }
        return oss.str();
    }
}