#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <kotono_common/Event.h>
class GWindow final
{
	friend void framebuffersize_callback_(GLFWwindow*, int, int);

public:
	void Init(); 
	void Cleanup();

	// Executes vkDeviceWaitIdle(VkDevice) if true, else executes glfwPollEvents()
	bool GetShouldClose(VkDevice device) const;
	// Sets whether the window should close at the end of the main loop
	void SetShouldClose(const bool shouldClose);

	GLFWwindow* GetGLFWWindow() const;
	const glm::uvec2& GetSize() const;
	UEvent<glm::uvec2>& GetEventWindowResized();

private:
	GLFWwindow* window_;
	bool shouldClose_;
	glm::uvec2 size_;
	UEvent<glm::uvec2> eventWindowResized_;
};

inline GWindow Window;
