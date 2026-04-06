#pragma once
#include <glm/fwd.hpp>
#include <kotono_io/serialize_base.h>

template <>
struct USerialize<glm::vec2>
{
	void operator()(nlohmann::json& json, const glm::vec2& v) const;
};

template <>
struct USerialize<glm::vec3>
{
	void operator()(nlohmann::json& json, const glm::vec3& v) const;
};

template <>
struct USerialize<glm::uvec2>
{
	void operator()(nlohmann::json& json, const glm::uvec2& v) const;
};

template <>
struct USerialize<glm::ivec2>
{
	void operator()(nlohmann::json& json, const glm::ivec2& v) const;
};

template <>
struct USerialize<glm::quat>
{
	void operator()(nlohmann::json& json, const glm::quat& v) const;
};

template <>
struct UDeserialize<glm::vec2>
{
	void operator()(const nlohmann::json& json, glm::vec2& v) const;
};

template <>
struct UDeserialize<glm::vec3>
{
	void operator()(const nlohmann::json& json, glm::vec3& v) const;
};

template <>
struct UDeserialize<glm::uvec2>
{
	void operator()(const nlohmann::json& json, glm::uvec2& v) const;
};

template <>
struct UDeserialize<glm::ivec2>
{
	void operator()(const nlohmann::json& json, glm::ivec2& v) const;
};

template <>
struct UDeserialize<glm::quat>
{
	void operator()(const nlohmann::json& json, glm::quat& v) const;
};
