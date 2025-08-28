#pragma once
#include <kotono_framework/glm_includes.h>
struct KtColor final
{
	struct RGB { float r, g, b; };
	struct HSV { float h, s, v; };
	struct HSL { float h, s, l; };

	union
	{
		struct { float r, g, b; };
		RGB rgb;
		float data[3];
	};
	float a;

	constexpr KtColor(float red, float green, float blue, float alpha) :
		r(red), g(green), b(blue), a(alpha)
	{}

	constexpr KtColor(float red, float green, float blue) :
		r(red), g(green), b(blue), a(1.0f)
	{}

	constexpr KtColor WithRed(float red)     const noexcept { return { red, g, b, a }; }
	constexpr KtColor WithGreen(float green) const noexcept { return { r, green, b, a }; }
	constexpr KtColor WithBlue(float blue)   const noexcept { return { r, g, blue, a }; }
	constexpr KtColor WithAlpha(float alpha) const noexcept { return { r, g, b, alpha }; }
	constexpr KtColor WithValue(float value) const noexcept { return { r * value, g * value, b * value, a }; }

	static consteval KtColor Black()   noexcept { return { 0.0f, 0.0f, 0.0f, 1.0f }; }
	static consteval KtColor Blue()    noexcept { return { 0.0f, 0.0f, 1.0f, 1.0f }; }
	static consteval KtColor Cyan()    noexcept { return { 0.0f, 1.0f, 1.0f, 1.0f }; }
	static consteval KtColor Green()   noexcept { return { 0.0f, 1.0f, 0.0f, 1.0f }; }
	static consteval KtColor Magenta() noexcept { return { 1.0f, 0.0f, 1.0f, 1.0f }; }
	static consteval KtColor Red()     noexcept { return { 1.0f, 0.0f, 0.0f, 1.0f }; }
	static consteval KtColor White()   noexcept { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
	static consteval KtColor Yellow()  noexcept { return { 1.0f, 1.0f, 0.0f, 1.0f }; }

	static constexpr KtColor Mix(const KtColor& left, const KtColor& right) noexcept
	{
		return (left + right) / 2.0f;
	}

	constexpr bool IsVisible()     const noexcept { return a > 0.0f; }
	constexpr bool IsOpaque()      const noexcept { return a >= 1.0f; }
	constexpr bool IsTransparent() const noexcept { return a < 1.0f; }

	constexpr HSV GetHSV() const noexcept
	{
		float max = r;
		float min = r;

		if (g > max) max = g;
		if (b > max) max = b;

		if (g < min) min = g;
		if (b < min) min = b;

		const float delta = max - min;

		HSV hsv{};
		if (delta == 0.0f) hsv.h = 0.0f;
		else if (max == r) hsv.h = 60.0f * ((g - b) / delta);
		else if (max == g) hsv.h = 60.0f * ((b - r) / delta + 2.0f);
		else hsv.h = 60.0f * ((r - g) / delta + 4.0f);

		if (hsv.h < 0.0f) hsv.h += 360.0f;

		hsv.s = max == 0.0f ? 0.0f : (delta / max) * 100.0f;
		hsv.v = max * 100.0f;

		return hsv;
	}

	constexpr HSL GetHSL() const noexcept
	{
		float max = r;
		float min = r;

		if (g > max) max = g;
		if (b > max) max = b;

		if (g < min) min = g;
		if (b < min) min = b;

		const float delta = max - min;

		HSL hsl{};
		hsl.l = (max + min) / 2.0f * 100.0f;

		if (delta == 0.0f)
		{
			hsl.h = 0.0f;
			hsl.s = 0.0f;
		}
		else
		{
			float t = 2.0f * hsl.l / 100.0f - 1.0f;
			if (t < 0.0f) t = -t;
			hsl.s = delta / (1.0f - t) * 100.0f;

			if (max == r)
			{
				float temp = (g - b) / delta;
				if (temp < 0.0f) temp += 6.0f;
				hsl.h = 60.0f * temp;
			}
			else if (max == g) hsl.h = 60.0f * ((b - r) / delta + 2.0f);
			else hsl.h = 60.0f * ((r - g) / delta + 4);

			if (hsl.h < 0.0f) hsl.h += 360.0f;
		}

		return hsl;
	}

	static constexpr KtColor FromHex(uint32_t hex) noexcept
	{
		return {
			((hex >> 24) & 0xFF) / 255.0f,
			((hex >> 16) & 0xFF) / 255.0f,
			((hex >> 8) & 0xFF) / 255.0f,
			(hex & 0xFF) / 255.0f
		};
	}

	static constexpr KtColor FromHSV(HSV hsv) noexcept
	{
		const float h = hsv.h / 60.0f;
		const float s = hsv.s / 100.0f;
		const float v = hsv.v / 100.0f;

		const int i = static_cast<int>(h) % 6;
		const float f = h - static_cast<int>(h);
		const float p = v * (1.0f - s);
		const float q = v * (1.0f - f * s);
		const float t = v * (1.0f - (1.0f - f) * s);

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

		return { rgb.r, rgb.g, rgb.b, 1.0f };
	}

	static constexpr KtColor FromHSL(HSL hsl) noexcept
	{
		auto absf = [](float v) constexpr noexcept { return v < 0 ? -v : v; };
		auto modf = [](float x, float y) constexpr noexcept { return x - y * static_cast<int>(x / y); };

		const float h = hsl.h / 360.0f;
		const float s = hsl.s / 100.0f;
		const float l = hsl.l / 100.0f;

		const float c = (1.0f - absf(2.0f * l - 1.0f)) * s;
		const float x = c * (1.0f - absf(modf(h * 6.0f, 2.0f) - 1.0f));
		const float m = l - c / 2.0f;

		RGB rgb{};
		if (0.0f <= h && h < 1.0f / 6.0f) rgb = { c, x, 0.0f };
		else if (h < 2.0f / 6.0f) rgb = { x, c, 0.0f };
		else if (h < 3.0f / 6.0f) rgb = { 0.0f, c, x };
		else if (h < 4.0f / 6.0f) rgb = { 0.0f, x, c };
		else if (h < 5.0f / 6.0f) rgb = { x, 0.0f, c };
		else                      rgb = { c, 0.0f, x };

		rgb.r += m;
		rgb.g += m;
		rgb.b += m;

		return { rgb.r, rgb.g, rgb.b, 1.0f };
	}

	constexpr operator glm::vec3() const noexcept
	{
		return { r, g, b };
	}

	constexpr operator glm::vec4() const noexcept
	{
		return { r, g, b, a };
	}

	constexpr KtColor operator+(const KtColor& other) const noexcept
	{
		return { r + other.r, g + other.g, b + other.b, a };
	}

	constexpr KtColor operator+(float f) const noexcept
	{
		return { r + f, g + f, b + f, a };
	}

	constexpr KtColor operator-(const KtColor& other) const noexcept
	{
		return { r - other.r, g - other.g, b - other.b, a };
	}

	constexpr KtColor operator-(float f) const noexcept
	{
		return { r - f, g - f, b - f, a };
	}

	constexpr KtColor operator*(const KtColor& other) const noexcept
	{
		return { r * other.r, g * other.g, b * other.b, a };
	}

	constexpr KtColor operator*(float f) const noexcept
	{
		return { r * f, g * f, b * f, a };
	}

	constexpr KtColor operator/(const KtColor& other) const noexcept
	{
		return { r / other.r, g / other.g, b / other.b, a };
	}

	constexpr KtColor operator/(float f) const noexcept
	{
		return { r / f, g / f, b / f, a };
	}

	constexpr bool operator==(const KtColor& other) const noexcept
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	constexpr bool operator!=(const KtColor& other) const noexcept
	{
		return r != other.r || g != other.g || b != other.b || a != other.a;
	}
};
