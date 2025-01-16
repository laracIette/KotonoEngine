#include "Color.h"
#include <algorithm>
#include <cmath>

Color::Color() :
    _rgb({ 1.0f, 1.0f, 1.0f }),
    _alpha(1.0f)
{
}

const Color::RGB& Color::GetRGB() const
{
    return _rgb;
}

const Color::HSV Color::GetHSV() const
{
    float max = std::max({ _rgb.Red, _rgb.Green, _rgb.Blue });
    float min = std::min({ _rgb.Red, _rgb.Green, _rgb.Blue });
    float delta = max - min;

    HSV hsv{};
    if (delta == 0) hsv.Hue = 0;
    else if (max == _rgb.Red) hsv.Hue = 60.0f * fmod((_rgb.Green - _rgb.Blue) / delta, 6.0f);
    else if (max == _rgb.Green) hsv.Hue = 60.0f * ((_rgb.Blue - _rgb.Red) / delta + 2.0f);
    else hsv.Hue = 60.0f * ((_rgb.Red - _rgb.Green) / delta + 4);

    if (hsv.Hue < 0) hsv.Hue += 360.0f;

    hsv.Saturation = max == 0 ? 0 : (delta / max) * 100.0f;
    hsv.Value = max * 100.0f;

    return hsv;
}

const Color::HSL Color::GetHSL() const
{
    float max = std::max({ _rgb.Red, _rgb.Green, _rgb.Blue });
    float min = std::min({ _rgb.Red, _rgb.Green, _rgb.Blue });
    float delta = max - min;

    HSL hsl{};
    hsl.Lightness = (max + min) / 2.0f * 100.0f;

    if (delta == 0)
    {
        hsl.Hue = hsl.Saturation = 0;
    }
    else
    {
        hsl.Saturation = delta / (1.0f - std::fabs(2.0f * hsl.Lightness / 100.0f - 1.0f)) * 100.0f;

        if (max == _rgb.Red) hsl.Hue = 60.0f * fmod((_rgb.Green - _rgb.Blue) / delta, 6.0f);
        else if (max == _rgb.Green) hsl.Hue = 60.0f * ((_rgb.Blue - _rgb.Red) / delta + 2.0f);
        else hsl.Hue = 60.0f * ((_rgb.Red - _rgb.Green) / delta + 4);

        if (hsl.Hue < 0) hsl.Hue += 360.0f;
    }

    return hsl;
}

const float Color::GetAlpha() const
{
    return _alpha;
}

void Color::SetRGB(const RGB& rgb)
{
    _rgb.Red = std::clamp(rgb.Red, 0.0f, 1.0f);
    _rgb.Green = std::clamp(rgb.Green, 0.0f, 1.0f);
    _rgb.Blue = std::clamp(rgb.Blue, 0.0f, 1.0f);
}

void Color::SetHSV(const HSV& hsv)
{
    float h = hsv.Hue / 60.0f;
    float s = hsv.Saturation / 100.0f;
    float v = hsv.Value / 100.0f;

    int i = static_cast<int>(h) % 6;
    float f = h - static_cast<int>(h);
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);

    switch (i)
    {
    case 0: _rgb = { v, t, p }; break;
    case 1: _rgb = { q, v, p }; break;
    case 2: _rgb = { p, v, t }; break;
    case 3: _rgb = { p, q, v }; break;
    case 4: _rgb = { t, p, v }; break;
    case 5: _rgb = { v, p, q }; break;
    }
}

void Color::SetHSL(const HSL& hsl)
{
    float h = hsl.Hue / 360.0f;
    float s = hsl.Saturation / 100.0f;
    float l = hsl.Lightness / 100.0f;

    float c = (1.0f - std::fabs(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - std::fabs(fmod(h * 6.0f, 2.0f) - 1.0f));
    float m = l - c / 2.0f;

    if (0 <= h && h < 1.0f / 6.0f) _rgb = { c, x, 0 };
    else if (1.0f / 6.0f <= h && h < 2.0f / 6.0f) _rgb = { x, c, 0 };
    else if (2.0f / 6.0f <= h && h < 3.0f / 6.0f) _rgb = { 0, c, x };
    else if (3.0f / 6.0f <= h && h < 4.0f / 6.0f) _rgb = { 0, x, c };
    else if (4.0f / 6.0f <= h && h < 5.0f / 6.0f) _rgb = { x, 0, c };
    else _rgb = { c, 0, x };

    _rgb.Red += m;
    _rgb.Green += m;
    _rgb.Blue += m;
}

void Color::SetAlpha(const float alpha)
{
    _alpha = alpha;
}

const bool Color::IsVisible() const
{
    return _alpha > 0.0f;
}

const bool Color::IsOpaque() const
{
    return _alpha == 1.0f;
}

const bool Color::IsTransparent() const
{
    return _alpha < 1.0f;
}


