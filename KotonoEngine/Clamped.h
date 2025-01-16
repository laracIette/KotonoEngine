#pragma once
template <typename T>
class Clamped
{
public:
	Clamped(const T& min, const T& max);

	Clamped& operator=(const T& value)
	{
		_value = Clamp(value);
		return *this;
	}

	operator T() const
	{
		return _value;
	}

private:
	T _value;
	T _min;
	T _max;

	const T Clamp(const T& value) const;
};