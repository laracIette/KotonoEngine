#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
struct GLFWwindow;
class UWindow final
{
	friend void framebuffersize_callback_(GLFWwindow*, i32, i32);

public:
	void Init(glm::uvec2 const& extent);
	void Cleanup() const;

	// Executes glfwPollEvents() when returns false
	auto GetShouldClose() const -> b8;

	auto GetGLFWWindow() const -> GLFWwindow* { return window_; }
	auto GetExtent() const -> glm::uvec2 const& { return extent_; }
	auto GetEventWindowResized() -> UEvent<glm::uvec2>& { return eventWindowResized_; }

private:
	void OnFramebufferSizeChanged(GLFWwindow* window, glm::uvec2 const& size);

private:
	GLFWwindow* window_;
	glm::uvec2 extent_;
	UEvent<glm::uvec2> eventWindowResized_;
};
