#pragma once
#include <GLFW/glfw3.h>
#include "glm_includes.h"
#include "Event.h"
class KtWindow final
{
	friend class KtFramework;

private:
	void Init(); 
	void Cleanup();

public:
	// Executes vkDeviceWaitIdle(VkDevice) if true, else executes glfwPollEvents()
	bool GetShouldClose() const;
	// Sets whether the window should close at the end of the main loop
	void SetShouldClose(const bool shouldClose);

	GLFWwindow* GetGLFWWindow() const;
	const glm::uvec2& GetSize() const;
	KtEvent<>& GetEventWindowResized();

	void SetSize(const glm::uvec2& size);

private:
	GLFWwindow* window_;
	bool shouldClose_;
	glm::uvec2 size_;
	KtEvent<> eventWindowResized_;
};

