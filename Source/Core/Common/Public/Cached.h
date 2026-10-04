#pragma once
#include "types.h"
#include <functional>
template <typename T>
class UCached final
{
public:
	using ValueFunction = std::function<T()>;

public:
	UCached() : 
		value_{},
		valueFunction_{},
		isDirty_{ true }
	{}

	UCached(ValueFunction&& valueFunction) : 
		value_{},
		valueFunction_{ std::move(valueFunction) },
		isDirty_{ true }
	{}

	// Get the current value without updating it
	T const& GetValue() const noexcept
	{
		return value_;
	}

	// Compute the value if dirty
	void TryUpdateValue()
	{
		if (isDirty_)
		{
			isDirty_ = false;
			value_ = valueFunction_();
		}
	}

	constexpr b8 GetIsDirty() const noexcept
	{
		return isDirty_;
	}

	constexpr void MarkDirty() noexcept
	{
		isDirty_ = true;
	}

	// Try to update and return the value
	operator T&()
	{
		TryUpdateValue();
		return value_;
	}

	void operator=(T const& value)
	{
		value_ = value;
	}

private:
	T value_;
	b8 isDirty_;
	ValueFunction valueFunction_;
};

