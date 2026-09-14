#pragma once
#include "Event.h"
#include <type_traits>
template <typename T>
class UNotify final
{
private:
    using ValueType = T;

    using ReturnType = std::conditional_t<
        (sizeof(ValueType) <= 16) && std::is_trivially_copyable_v<ValueType>,
        ValueType,
        const ValueType&
    >;

public:
    UNotify() 
        : value_{}
        , eventValueChanged_{} 
    {}
    
    UNotify(ValueType const& value)
        : value_{ value }
        , eventValueChanged_{} 
    {}
    
    UNotify(ValueType&& value)
        : value_{ std::move(value) }
        , eventValueChanged_{} 
    {}

    UNotify& operator=(ValueType const& value)
    {
        value_ = value; 
        BroadcastValueChanged();
        return *this;
    }

    UNotify& operator=(ValueType&& value)
    {
        value_ = std::move(value);
        BroadcastValueChanged();
        return *this;
    }

    UEvent<ValueType>& GetEventValueChanged()
    {
        return eventValueChanged_;
    }

    ValueType* operator->() const noexcept
    {
        return &value_;
    }

    operator ReturnType() const noexcept
    {
        return value_;
    }

    bool operator==(UNotify const& other) const noexcept
    {
        return value_ == other.value_;
    }

    bool operator==(ValueType const& value) const noexcept
    {
        return value_ == value;
    }

private:
    void BroadcastValueChanged() const
    {
        eventValueChanged_.Broadcast(value_);
    }

private:
    ValueType value_;
    UEvent<ValueType> eventValueChanged_;
};