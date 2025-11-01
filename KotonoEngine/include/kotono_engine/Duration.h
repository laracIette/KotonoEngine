#pragma once
#include <cstdint>
#include <variant>

struct UDuration final
{
    static constexpr UDuration FromSeconds(const float seconds) noexcept
    {
        return { seconds };
    }

    static constexpr UDuration FromUpdates(const int64_t updates) noexcept
    {
        return { updates };
    }

    constexpr UDuration operator+(const UDuration& other) const
    {
        return std::visit(
            [](auto lhs, auto rhs)
            {
                return UDuration{ lhs + rhs };
            }, 
            value, other.value
        );
    }

    constexpr UDuration& operator+=(const UDuration& other)
    {
        *this = *this + other; 
        return *this;
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

    constexpr UDuration& operator-=(const UDuration& other)
    {
        *this = *this - other;
        return *this;
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

    constexpr bool IsSeconds() const noexcept
    {
        return std::holds_alternative<float>(value);
    }

    constexpr bool IsUpdates() const noexcept
    {
        return std::holds_alternative<int64_t>(value);
    }

    std::variant<float, int64_t> value;
};
