#pragma once
#include <kotono_framework/glm_includes.h>
struct UColor final
{
	struct RGB { float red, green, blue; };
	struct HSV { float hue, saturation, value; };
	struct HSL { float hue, saturation, lightness; };

	RGB rgb{ 0.0f, 0.0f, 0.0f };
	float alpha{ 1.0f };

	HSV GetHSV() const
	{
		float max = std::max({ rgb.red, rgb.green, rgb.blue });
		float min = std::min({ rgb.red, rgb.green, rgb.blue });
		float delta = max - min;

		HSV hsv{};
		if (delta == 0) hsv.hue = 0;
		else if (max == rgb.red) hsv.hue = 60.0f * fmod((rgb.green - rgb.blue) / delta, 6.0f);
		else if (max == rgb.green) hsv.hue = 60.0f * ((rgb.blue - rgb.red) / delta + 2.0f);
		else hsv.hue = 60.0f * ((rgb.red - rgb.green) / delta + 4);

		if (hsv.hue < 0) hsv.hue += 360.0f;

		hsv.saturation = max == 0 ? 0 : (delta / max) * 100.0f;
		hsv.value = max * 100.0f;

		return hsv;
	}

	HSL GetHSL() const
	{
		float max = std::max({ rgb.red, rgb.green, rgb.blue });
		float min = std::min({ rgb.red, rgb.green, rgb.blue });
		float delta = max - min;

		HSL hsl{};
		hsl.lightness = (max + min) / 2.0f * 100.0f;

		if (delta == 0)
		{
			hsl.hue = hsl.saturation = 0;
		}
		else
		{
			hsl.saturation = delta / (1.0f - std::fabs(2.0f * hsl.lightness / 100.0f - 1.0f)) * 100.0f;

			if (max == rgb.red) hsl.hue = 60.0f * fmod((rgb.green - rgb.blue) / delta, 6.0f);
			else if (max == rgb.green) hsl.hue = 60.0f * ((rgb.blue - rgb.red) / delta + 2.0f);
			else hsl.hue = 60.0f * ((rgb.red - rgb.green) / delta + 4);

			if (hsl.hue < 0) hsl.hue += 360.0f;
		}

		return hsl;
	}

	constexpr bool GetIsVisible() const noexcept
	{
		return alpha > 0.0f;
	}

	constexpr bool GetIsOpaque() const noexcept
	{
		return alpha >= 1.0f;
	}

	constexpr bool GetIsTransparent() const noexcept
	{
		return alpha < 1.0f;
	}

	static constexpr UColor FromHSV(const HSV& hsv, const float alpha = 1.0f) noexcept
	{
		float h = hsv.hue / 60.0f;
		float s = hsv.saturation / 100.0f;
		float v = hsv.value / 100.0f;

		int i = static_cast<int>(h) % 6;
		float f = h - static_cast<int>(h);
		float p = v * (1.0f - s);
		float q = v * (1.0f - f * s);
		float t = v * (1.0f - (1.0f - f) * s);

		RGB rgb{};
		switch (i)
		{
		case 0: rgb = { v, t, p }; break;
		case 1: rgb = { q, v, p }; break;
		case 2: rgb = { p, v, t }; break;
		case 3: rgb = { p, q, v }; break;
		case 4: rgb = { t, p, v }; break;
		case 5: rgb = { v, p, q }; break;
		}

		return { rgb, alpha };
	}

	static UColor FromHSL(const HSL& hsl, const float alpha = 1.0f) noexcept
	{
		float h = hsl.hue / 360.0f;
		float s = hsl.saturation / 100.0f;
		float l = hsl.lightness / 100.0f;

		float c = (1.0f - std::fabs(2.0f * l - 1.0f)) * s;
		float x = c * (1.0f - std::fabs(fmod(h * 6.0f, 2.0f) - 1.0f));
		float m = l - c / 2.0f;

		RGB rgb{};
		if (0 <= h && h < 1.0f / 6.0f) rgb = { c, x, 0 };
		else if (1.0f / 6.0f <= h && h < 2.0f / 6.0f) rgb = { x, c, 0 };
		else if (2.0f / 6.0f <= h && h < 3.0f / 6.0f) rgb = { 0, c, x };
		else if (3.0f / 6.0f <= h && h < 4.0f / 6.0f) rgb = { 0, x, c };
		else if (4.0f / 6.0f <= h && h < 5.0f / 6.0f) rgb = { x, 0, c };
		else rgb = { c, 0, x };

		rgb.red += m;
		rgb.green += m;
		rgb.blue += m;

		return { rgb, alpha };
	}

	constexpr operator glm::vec4() const noexcept
	{
		return glm::vec4(rgb.red, rgb.green, rgb.blue, alpha);
	}
};
