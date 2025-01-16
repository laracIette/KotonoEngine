#pragma once
#include <glm/glm.hpp>
class Color final
{
public:
	Color();

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

	// Get whether the alpha of the color is superior to 0.
	const bool IsVisible() const;
	// Get whether the alpha of the color is equal to 1.
	const bool IsOpaque() const;
	// Get whether the alpha of the color is inferior to 1.
	const bool IsTransparent() const;

	operator glm::vec4() const
	{
		return glm::vec4(_rgb.Red, _rgb.Green, _rgb.Blue, _alpha);
	}

private:
	RGB _rgb;
	float _alpha;
};

