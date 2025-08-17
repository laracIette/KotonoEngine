#pragma once
#include <cstdint>
#include <cassert>
struct UDuration final
{
    enum class Type : char
    {
        Time,
        Updates
    };

    static constexpr UDuration FromTime(const float time)
    {
        return UDuration{ .time = time, .type = Type::Time };
    }

    static constexpr UDuration FromUpdates(const int64_t updates)
    {
        return UDuration{ .updates = updates, .type = Type::Updates };
    }

    union
    {
        float    time;
        int64_t  updates;
    };
    Type type;

    UDuration operator-(const UDuration& other) const
    {
        assert(type == other.type && "Type mismatch in UDuration comparison");

        if (type == Type::Time)
        {
            return UDuration{ .time = time - other.time, .type = Type::Time };
        }
        else
        {
            return UDuration{ .updates = updates - other.updates, .type = Type::Updates };
        }
    }

    bool operator<(const UDuration& other) const
    {
        assert(type == other.type && "Type mismatch in UDuration comparison");
        
        if (type == Type::Time)
        {
            return time < other.time;
        }
        else
        {
            return updates < other.updates;
        }
    }
};

