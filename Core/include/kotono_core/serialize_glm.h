#pragma once
#include <glm/fwd.hpp>
#include <kotono_io/serialize_base.h>

template <>
struct USerialize<glm::vec2>
{
	void operator()(nlohmann::json& json, glm::vec2 const& v) const;
};

template <>
struct USerialize<glm::vec3>
{
	void operator()(nlohmann::json& json, glm::vec3 const& v) const;
};

template <>
struct USerialize<glm::uvec2>
{
	void operator()(nlohmann::json& json, glm::uvec2 const& v) const;
};

template <>
struct USerialize<glm::ivec2>
{
	void operator()(nlohmann::json& json, glm::ivec2 const& v) const;
};

template <>
struct USerialize<glm::quat>
{
	void operator()(nlohmann::json& json, glm::quat const& v) const;
};

template <>
struct UDeserialize<glm::vec2>
{
	void operator()(nlohmann::json const& json, glm::vec2& v) const;
};

template <>
struct UDeserialize<glm::vec3>
{
	void operator()(nlohmann::json const& json, glm::vec3& v) const;
};

template <>
struct UDeserialize<glm::uvec2>
{
	void operator()(nlohmann::json const& json, glm::uvec2& v) const;
};

template <>
struct UDeserialize<glm::ivec2>
{
	void operator()(nlohmann::json const& json, glm::ivec2& v) const;
};

template <>
struct UDeserialize<glm::quat>
{
	void operator()(nlohmann::json const& json, glm::quat& v) const;
};
