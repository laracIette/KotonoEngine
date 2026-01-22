#pragma once
#include <kotono_interface/Widget.h>
template <typename T>
class WValueBox : public WWidget
{
public:
	using ValueType = T;

public:
	WValueBox(T* value) : 
		value_(value) 
	{}

	T* Get()
	{
		return value_;
	}

	const T& GetValue() const 
	{ 
		return *value_;
	}
	
	void SetValue(const T& value) 
	{ 
		*value_ = value;
	}

private:
	T* value_;
};