#include "serialize_glm.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

void USerialize<glm::vec2>::operator()(nlohmann::json& json, const glm::vec2& v) const
{
	USerialize<float>{}(get(json, "x"), v.x);
	USerialize<float>{}(get(json, "y"), v.y);
}

void USerialize<glm::vec3>::operator()(nlohmann::json& json, const glm::vec3& v) const
{
	USerialize<float>{}(get(json, "x"), v.x);
	USerialize<float>{}(get(json, "y"), v.y);
	USerialize<float>{}(get(json, "z"), v.z);
}

void USerialize<glm::uvec2>::operator()(nlohmann::json& json, const glm::uvec2& v) const
{
	USerialize<u32>{}(get(json, "x"), v.x);
	USerialize<u32>{}(get(json, "y"), v.y);
}

void USerialize<glm::ivec2>::operator()(nlohmann::json& json, const glm::ivec2& v) const
{
	USerialize<i32>{}(get(json, "x"), v.x);
	USerialize<i32>{}(get(json, "y"), v.y);
}

void USerialize<glm::quat>::operator()(nlohmann::json& json, const glm::quat& v) const
{
	USerialize<float>{}(get(json, "w"), v.w);
	USerialize<float>{}(get(json, "x"), v.x);
	USerialize<float>{}(get(json, "y"), v.y);
	USerialize<float>{}(get(json, "z"), v.z);
}

void UDeserialize<glm::vec2>::operator()(const nlohmann::json& json, glm::vec2& v) const
{
	UDeserialize<float>{}(get(json, "x"), v.x);
	UDeserialize<float>{}(get(json, "y"), v.y);
}

void UDeserialize<glm::vec3>::operator()(const nlohmann::json& json, glm::vec3& v) const
{
	UDeserialize<float>{}(get(json, "x"), v.x);
	UDeserialize<float>{}(get(json, "y"), v.y);
	UDeserialize<float>{}(get(json, "z"), v.z);
}

void UDeserialize<glm::uvec2>::operator()(const nlohmann::json& json, glm::uvec2& v) const
{
	UDeserialize<u32>{}(get(json, "x"), v.x);
	UDeserialize<u32>{}(get(json, "y"), v.y);
}

void UDeserialize<glm::ivec2>::operator()(const nlohmann::json& json, glm::ivec2& v) const
{
	UDeserialize<i32>{}(get(json, "x"), v.x);
	UDeserialize<i32>{}(get(json, "y"), v.y);
}

void UDeserialize<glm::quat>::operator()(const nlohmann::json& json, glm::quat& v) const
{
	UDeserialize<float>{}(get(json, "w"), v.w);
	UDeserialize<float>{}(get(json, "x"), v.x);
	UDeserialize<float>{}(get(json, "y"), v.y);
	UDeserialize<float>{}(get(json, "z"), v.z);
}
