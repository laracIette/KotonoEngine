#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <kotono_common/types.h>
#include <kotono_io/serialize_base.h>
struct UColor final
{
	struct RGB { f32 r, g, b; };
	struct HSV { f32 h, s, v; };
	struct HSL { f32 h, s, l; };

	union
	{
		struct { f32 r, g, b; };
		RGB rgb;
		f32 data[3];
	};
	f32 a;

	constexpr UColor() 
		: r{ 0.0f }, g{ 0.0f }, b{ 0.0f }, a{ 1.0f }
	{}

	constexpr UColor(f32 red, f32 green, f32 blue, f32 alpha) 
		: r{ red }, g{ green }, b{ blue }, a{ alpha }
	{}

	constexpr UColor(f32 red, f32 green, f32 blue) 
		: r{ red }, g{ green }, b{ blue }, a{ 1.0f }
	{}

	constexpr UColor WithRed(f32 red)		const noexcept { return { red, g, b, a }; }
	constexpr UColor WithGreen(f32 green)	const noexcept { return { r, green, b, a }; }
	constexpr UColor WithBlue(f32 blue)		const noexcept { return { r, g, blue, a }; }
	constexpr UColor WithAlpha(f32 alpha)	const noexcept { return { r, g, b, alpha }; }
	constexpr UColor WithValue(f32 value)	const noexcept { return { r * value, g * value, b * value, a }; }
	
	static constexpr UColor Mix(const UColor& left, const UColor& right) noexcept { return (left + right) / 2.0f; }

	constexpr bool IsVisible()		const noexcept { return a > 0.0f; }
	constexpr bool IsOpaque()		const noexcept { return a >= 1.0f; }
	constexpr bool IsTranslucent()	const noexcept { return a < 1.0f; }

	constexpr HSV GetHSV() const noexcept
	{
		f32 max = r;
		f32 min = r;

		if (g > max) max = g;
		if (b > max) max = b;

		if (g < min) min = g;
		if (b < min) min = b;

		const f32 delta = max - min;

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
		f32 max = r;
		f32 min = r;

		if (g > max) max = g;
		if (b > max) max = b;

		if (g < min) min = g;
		if (b < min) min = b;

		const f32 delta = max - min;

		HSL hsl{};
		hsl.l = (max + min) / 2.0f * 100.0f;

		if (delta == 0.0f)
		{
			hsl.h = 0.0f;
			hsl.s = 0.0f;
		}
		else
		{
			f32 t = 2.0f * hsl.l / 100.0f - 1.0f;
			if (t < 0.0f) t = -t;
			hsl.s = delta / (1.0f - t) * 100.0f;

			if (max == r)
			{
				f32 temp = (g - b) / delta;
				if (temp < 0.0f) temp += 6.0f;
				hsl.h = 60.0f * temp;
			}
			else if (max == g) hsl.h = 60.0f * ((b - r) / delta + 2.0f);
			else hsl.h = 60.0f * ((r - g) / delta + 4);

			if (hsl.h < 0.0f) hsl.h += 360.0f;
		}

		return hsl;
	}

	static constexpr UColor FromHex(u32 hex) noexcept
	{
		return {
			((hex >> 24) & 0xFF) / 255.0f,
			((hex >> 16) & 0xFF) / 255.0f,
			((hex >> 8) & 0xFF) / 255.0f,
			(hex & 0xFF) / 255.0f
		};
	}

	static constexpr UColor FromHSV(HSV hsv) noexcept
	{
		const f32 h = hsv.h / 60.0f;
		const f32 s = hsv.s / 100.0f;
		const f32 v = hsv.v / 100.0f;

		const i32 i = static_cast<i32>(h) % 6;
		const f32 f = h - static_cast<i32>(h);
		const f32 p = v * (1.0f - s);
		const f32 q = v * (1.0f - f * s);
		const f32 t = v * (1.0f - (1.0f - f) * s);

		RGB rgb;
		switch (i)
		{
		case 0: rgb = { v, t, p }; break;
		case 1: rgb = { q, v, p }; break;
		case 2: rgb = { p, v, t }; break;
		case 3: rgb = { p, q, v }; break;
		case 4: rgb = { t, p, v }; break;
		case 5: rgb = { v, p, q }; break;
		default: rgb = {}; break;
		}

		return { rgb.r, rgb.g, rgb.b, 1.0f };
	}

	static constexpr UColor FromHSL(HSL hsl) noexcept
	{
		auto absf = [](f32 v) constexpr noexcept { return v < 0 ? -v : v; };
		auto modf = [](f32 x, f32 y) constexpr noexcept { return x - y * static_cast<i32>(x / y); };

		const f32 h = hsl.h / 360.0f;
		const f32 s = hsl.s / 100.0f;
		const f32 l = hsl.l / 100.0f;

		const f32 c = (1.0f - absf(2.0f * l - 1.0f)) * s;
		const f32 x = c * (1.0f - absf(modf(h * 6.0f, 2.0f) - 1.0f));
		const f32 m = l - c / 2.0f;

		RGB rgb;
		if (0.0f <= h && h < 1.0f / 6.0f) rgb = { c, x, 0.0f };
		else if (h < 2.0f / 6.0f)         rgb = { x, c, 0.0f };
		else if (h < 3.0f / 6.0f)         rgb = { 0.0f, c, x };
		else if (h < 4.0f / 6.0f)         rgb = { 0.0f, x, c };
		else if (h < 5.0f / 6.0f)         rgb = { x, 0.0f, c };
		else                              rgb = { c, 0.0f, x };

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

	constexpr UColor operator+(const UColor& other) const noexcept
	{
		return { r + other.r, g + other.g, b + other.b, a };
	}

	constexpr UColor operator+(f32 f) const noexcept
	{
		return { r + f, g + f, b + f, a };
	}

	constexpr UColor operator-(const UColor& other) const noexcept
	{
		return { r - other.r, g - other.g, b - other.b, a };
	}

	constexpr UColor operator-(f32 f) const noexcept
	{
		return { r - f, g - f, b - f, a };
	}

	constexpr UColor operator*(const UColor& other) const noexcept
	{
		return { r * other.r, g * other.g, b * other.b, a };
	}

	constexpr UColor operator*(f32 f) const noexcept
	{
		return { r * f, g * f, b * f, a };
	}

	constexpr UColor operator/(const UColor& other) const noexcept
	{
		return { r / other.r, g / other.g, b / other.b, a };
	}

	constexpr UColor operator/(f32 f) const noexcept
	{
		return { r / f, g / f, b / f, a };
	}

	constexpr bool operator==(const UColor& other) const noexcept
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	constexpr bool operator!=(const UColor& other) const noexcept
	{
		return r != other.r || g != other.g || b != other.b || a != other.a;
	}
};

namespace Colors
{
	inline constexpr UColor Black		{ 0.0f, 0.0f, 0.0f, 1.0f };
	inline constexpr UColor Blue		{ 0.0f, 0.0f, 1.0f, 1.0f };
	inline constexpr UColor Cyan		{ 0.0f, 1.0f, 1.0f, 1.0f };
	inline constexpr UColor Green		{ 0.0f, 1.0f, 0.0f, 1.0f };
	inline constexpr UColor Magenta		{ 1.0f, 0.0f, 1.0f, 1.0f };
	inline constexpr UColor Red			{ 1.0f, 0.0f, 0.0f, 1.0f };
	inline constexpr UColor Transparent	{ 0.0f, 0.0f, 0.0f, 0.0f };
	inline constexpr UColor White		{ 1.0f, 1.0f, 1.0f, 1.0f };
	inline constexpr UColor Yellow		{ 1.0f, 1.0f, 0.0f, 1.0f };
}

template<>
struct USerialize<UColor>
{
	void operator()(nlohmann::json& json, const UColor& v) const
	{
		USerialize<f32>{}(get(json, "r"), v.r);
		USerialize<f32>{}(get(json, "g"), v.g);
		USerialize<f32>{}(get(json, "b"), v.b);
		USerialize<f32>{}(get(json, "a"), v.a);
	}
};

template<>
struct UDeserialize<UColor>
{
	void operator()(const nlohmann::json& json, UColor& v) const
	{
		UDeserialize<f32>{}(get(json, "r"), v.r);
		UDeserialize<f32>{}(get(json, "g"), v.g);
		UDeserialize<f32>{}(get(json, "b"), v.b);
		UDeserialize<f32>{}(get(json, "a"), v.a);
	}
};
