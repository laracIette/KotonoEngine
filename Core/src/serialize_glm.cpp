#include "serialize_glm.h"

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_uint2.hpp>

void USerialize<glm::vec2>::operator()(nlohmann::json& json, glm::vec2 const& v) const
{
	USerialize<f32>{}(get(json, "x"), v.x);
	USerialize<f32>{}(get(json, "y"), v.y);
}

void USerialize<glm::vec3>::operator()(nlohmann::json& json, glm::vec3 const& v) const
{
	USerialize<f32>{}(get(json, "x"), v.x);
	USerialize<f32>{}(get(json, "y"), v.y);
	USerialize<f32>{}(get(json, "z"), v.z);
}

void USerialize<glm::uvec2>::operator()(nlohmann::json& json, glm::uvec2 const& v) const
{
	USerialize<u32>{}(get(json, "x"), v.x);
	USerialize<u32>{}(get(json, "y"), v.y);
}

void USerialize<glm::ivec2>::operator()(nlohmann::json& json, glm::ivec2 const& v) const
{
	USerialize<i32>{}(get(json, "x"), v.x);
	USerialize<i32>{}(get(json, "y"), v.y);
}

void USerialize<glm::quat>::operator()(nlohmann::json& json, glm::quat const& v) const
{
	USerialize<f32>{}(get(json, "w"), v.w);
	USerialize<f32>{}(get(json, "x"), v.x);
	USerialize<f32>{}(get(json, "y"), v.y);
	USerialize<f32>{}(get(json, "z"), v.z);
}

void UDeserialize<glm::vec2>::operator()(nlohmann::json const& json, glm::vec2& v) const
{
	UDeserialize<f32>{}(get(json, "x"), v.x);
	UDeserialize<f32>{}(get(json, "y"), v.y);
}

void UDeserialize<glm::vec3>::operator()(nlohmann::json const& json, glm::vec3& v) const
{
	UDeserialize<f32>{}(get(json, "x"), v.x);
	UDeserialize<f32>{}(get(json, "y"), v.y);
	UDeserialize<f32>{}(get(json, "z"), v.z);
}

void UDeserialize<glm::uvec2>::operator()(nlohmann::json const& json, glm::uvec2& v) const
{
	UDeserialize<u32>{}(get(json, "x"), v.x);
	UDeserialize<u32>{}(get(json, "y"), v.y);
}

void UDeserialize<glm::ivec2>::operator()(nlohmann::json const& json, glm::ivec2& v) const
{
	UDeserialize<i32>{}(get(json, "x"), v.x);
	UDeserialize<i32>{}(get(json, "y"), v.y);
}

void UDeserialize<glm::quat>::operator()(nlohmann::json const& json, glm::quat& v) const
{
	UDeserialize<f32>{}(get(json, "w"), v.w);
	UDeserialize<f32>{}(get(json, "x"), v.x);
	UDeserialize<f32>{}(get(json, "y"), v.y);
	UDeserialize<f32>{}(get(json, "z"), v.z);
}
