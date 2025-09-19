#pragma once
#include <functional>
template <typename T>
class KtCached final
{
public:
	using ValueFunction = std::function<T()>;

public:
	explicit KtCached(ValueFunction&& valueFunction) : 
		value_(T{}),
		valueFunction_(std::move(valueFunction)),
		isDirty_(true) 
	{}

	// Get the current value without setting it if dirty
	T GetValue() const noexcept
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

	bool GetIsDirty() const noexcept
	{
		return isDirty_;
	}

	void MarkDirty() noexcept
	{
		isDirty_ = true;
	}

	operator T()
	{
		TryUpdateValue();
		return value_;
	}

private:
	T value_;
	bool isDirty_;
	ValueFunction valueFunction_;
};

