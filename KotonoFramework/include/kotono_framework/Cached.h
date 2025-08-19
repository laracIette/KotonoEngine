#pragma once
#include <functional>
template <typename T>
class KtCached final
{
public:
	using ValueFunction = std::function<T()>;

public:
	explicit KtCached(ValueFunction&& valueFunction) : 
		value_({}),
		valueFunction_(std::move(valueFunction)),
		isDirty_(true) 
	{}

	void MarkDirty()
	{
		isDirty_ = true;
	}

	operator T()
	{
		if (isDirty_)
		{
			isDirty_ = false;
			value_ = valueFunction_();
		}

		return value_;
	}

private:
	T value_;
	bool isDirty_;
	ValueFunction valueFunction_;
};

