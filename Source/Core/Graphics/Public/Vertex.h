#pragma once
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtx/hash.hpp>
#include <hash_utils.h>
#include <types.h>

struct UVertex final
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 tangent;

	auto operator==(UVertex const& other) const -> b8 = default;
};

template <>
struct std::hash<UVertex>
{
	auto operator()(UVertex const& v) const noexcept -> ::size
	{
		::size h{};
		h = combine(h, std::hash<glm::vec3>{}(v.position));
		h = combine(h, std::hash<glm::vec3>{}(v.normal));
		h = combine(h, std::hash<glm::vec2>{}(v.uv));
		h = combine(h, std::hash<glm::vec4>{}(v.tangent));
		return h;
	}
};