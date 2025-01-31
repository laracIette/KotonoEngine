#pragma once
template <float Min, float Max>
class ClampedFloat
{
public:
	constexpr ClampedFloat(const float value = 0.0f) : _value(Clamp(value)) {}
	constexpr operator float() const { return _value; }
	ClampedFloat& operator=(const float value)
	{
		_value = Clamp(value);
		return *this;
	}

private:
	float _value;
	static constexpr const float Clamp(const float value)
	{
		return (value < Min) ? Min : (Max < value) ? Max : value;
	}
};

