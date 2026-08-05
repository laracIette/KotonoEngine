#pragma once
#include <glm/ext/vector_uint2.hpp>
class UApplication final
{
public:
	void Run() const;

private:
	void Init() const;
	void Update() const;
	void Cleanup() const;

	void LogUPS() const;
	void OnWindowResized(const glm::uvec2 extent) const;
};