#pragma once
#include <glm/glm.hpp>
#include "VulkanInstance.h"

class Window final
{
public:
	Window();

	void Cleanup();

	void Init();

	GLFWwindow* GetGLFWWindow() const;

	const bool IsRunning() const;

	void SwapBuffers() const;

	const glm::uvec2& GetSize() const;

	void SetSize(const glm::uvec2& size);

private:
	GLFWwindow* _window;
	VulkanInstance _vulkanInstance;

	glm::uvec2 _size;

	void InitGLFW();
	void InitVulkan();
};

