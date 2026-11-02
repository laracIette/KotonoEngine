#pragma once
#include <algorithm>
#include <Clamped.h>
#include <types.h>
#include <glm/ext/vector_float4.hpp>

struct UCorners final
{
	UClamped<f32, 0.0f> topLeft;
	UClamped<f32, 0.0f> topRight;
	UClamped<f32, 0.0f> bottomLeft;
	UClamped<f32, 0.0f> bottomRight;
	
	static constexpr UCorners All(f32 v) noexcept
	{
		return { v, v, v, v };
	}
	
	static constexpr UCorners ClampToSize(UCorners const& corners, glm::vec2 const& size) noexcept
	{
		auto const maxCornerSize{ std::min(size.x / 2.0f, size.y / 2.0f) };
		return {
			std::min(corners.topLeft.Get(), maxCornerSize),
			std::min(corners.topRight.Get(), maxCornerSize),
			std::min(corners.bottomLeft.Get(), maxCornerSize),
			std::min(corners.bottomRight.Get(), maxCornerSize)
		};
	}
	
	constexpr operator glm::vec4() const noexcept
	{
		return { topLeft, topRight, bottomLeft, bottomRight };
	}
};