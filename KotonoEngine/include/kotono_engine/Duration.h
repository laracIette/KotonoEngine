#pragma once
#include <cstdint>
#include <variant>

struct UDuration final
{
    static constexpr UDuration FromSeconds(const float seconds)
    {
        return { seconds };
    }

    static constexpr UDuration FromUpdates(const int64_t updates)
    {
        return { updates };
    }

    constexpr UDuration operator-(const UDuration& other) const
    {
        return std::visit(
            [](auto lhs, auto rhs)
            {
                return UDuration{ lhs - rhs };
            }, 
            value, other.value
        );
    }

    constexpr bool operator<(const UDuration& other) const
    {
        return std::visit(
            [](auto lhs, auto rhs)
            {
                return lhs < rhs;
            }, 
            value, other.value
        );
    }

    std::variant<float, int64_t> value;
};
