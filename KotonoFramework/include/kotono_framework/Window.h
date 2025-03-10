#pragma once
#include <GLFW/glfw3.h>
#include "glm_includes.h"
class KtWindow
{
public:
	KtWindow();

	void Init(); 
	void Cleanup();

	// Executes vkDeviceWaitIdle(VkDevice) if true, else executes glfwPollEvents()
	const bool GetShouldClose() const;

	GLFWwindow* GetGLFWWindow() const;

	const glm::uvec2& GetSize() const;

	void SetSize(const glm::uvec2& size);

private:
	GLFWwindow* _window;

	glm::uvec2 _size;
};

