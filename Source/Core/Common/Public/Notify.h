#pragma once
#include "Event/Event.h"
#include "type_traits.h"

template <typename T>
class UNotify final
{
private:
    using ValueType = T;
	using EventType = UEvent<ValueType>;
    using ReturnType = optimal_t<ValueType>;

public:
    UNotify() 
        : value_{}
        , eventValueChanged_{} 
    {}
    
	UNotify(ValueType&& value)
		: value_{ std::move(value) }
		, eventValueChanged_{} 
    {}
    
    UNotify(ValueType const& value)
        : value_{ value }
        , eventValueChanged_{} 
    {}

	auto operator=(ValueType&& value) -> UNotify&
    {
    	value_ = std::move(value);
    	BroadcastValueChanged();
    	return *this;
    }

    auto operator=(ValueType const& value) -> UNotify&
    {
        value_ = value; 
        BroadcastValueChanged();
        return *this;
    }

    auto GetEventValueChanged() -> EventType&
    {
        return eventValueChanged_;
    }

    auto operator->() const noexcept -> ValueType*
    {
        return &value_;
    }

    operator ReturnType() const noexcept
    {
        return value_;
    }

    auto operator==(UNotify const& other) const noexcept -> b8
    {
        return value_ == other.value_;
    }

    auto operator==(ValueType const& value) const noexcept -> b8
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
    EventType eventValueChanged_;
};