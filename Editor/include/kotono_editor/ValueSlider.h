#pragma once
#include <kotono_interface/Widget.h>
template <typename T>
class WValueSlider : public WWidget
{
public:
	using ValueType = T;

public:
	WValueSlider() : value_(nullptr) {}
	WValueSlider(T* value) : value_(value) {}

	T& GetValue() const 
	{ 
		return *value_;
	}
	
	void SetValue(T& value) 
	{ 
		value_ = value;
	}

private:
	T* value_;
};