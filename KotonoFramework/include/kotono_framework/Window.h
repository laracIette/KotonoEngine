#pragma once
#include <GLFW/glfw3.h>
#include "glm_includes.h"
class KtWindow final
{
public:
	KtWindow();

	void Init(); 
	void Cleanup();

	// Executes vkDeviceWaitIdle(VkDevice) if true, else executes glfwPollEvents()
	const bool GetShouldClose() const;
	// Sets whether the window should close at the end of the main loop
	void SetShouldClose(const bool shouldClose);

	GLFWwindow* GetGLFWWindow() const;

	const glm::uvec2& GetSize() const;

	void SetSize(const glm::uvec2& size);

private:
	GLFWwindow* window_;

	bool shouldClose_;

	glm::uvec2 size_;
};

