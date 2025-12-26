#pragma once
#include <functional>
template <typename T>
class KtCached final
{
public:
	using ValueFunction = std::function<T()>;

public:
	KtCached() : 
		value_(T{}),
		valueFunction_(nullptr),
		isDirty_(true)
	{}

	KtCached(ValueFunction&& valueFunction) : 
		value_(T{}),
		valueFunction_(std::move(valueFunction)),
		isDirty_(true) 
	{}

	// Get the current value without updating it
	const T& Value() const noexcept
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

	constexpr bool IsDirty() const noexcept
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

	void operator=(const T& value)
	{
		value_ = value;
	}

private:
	T value_;
	bool isDirty_;
	ValueFunction valueFunction_;
};

