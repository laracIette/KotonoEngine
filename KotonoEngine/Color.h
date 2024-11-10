#pragma once
class Color final
{
public:
	struct RGB { float Red, Green, Blue; };
	struct HSV { float Hue, Saturation, Value; };
	struct HSL { float Hue, Saturation, Lightness; };

	// Getters
	const RGB& GetRGB() const;
	const HSV GetHSV() const;
	const HSL GetHSL() const;
	const float GetAlpha() const;

	// Setters
	void SetRGB(const RGB& rgb);
	void SetHSV(const HSV& hsv);
	void SetHSL(const HSL& hsl);
	void SetAlpha(const float alpha);

private:
	RGB _rgb;
	float _alpha;
};

