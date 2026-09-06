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
	void Init(); 
	void Cleanup();

	// Executes glfwPollEvents() when returns false
	b8 GetShouldClose() const;

	GLFWwindow*			GetGLFWWindow() const { return window_; }
	glm::uvec2 const&	GetSize() const { return size_; }
	UEvent<glm::uvec2>& GetEventWindowResized() { return eventWindowResized_; }

private:
	void OnFramebufferSizeChanged(glm::uvec2 const& size);

private:
	GLFWwindow* window_;
	glm::uvec2 size_;
	UEvent<glm::uvec2> eventWindowResized_;
};
