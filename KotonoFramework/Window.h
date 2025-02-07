#pragma once
#include <GLFW/glfw3.h>
#include "Context.h"
#include "glm_includes.h"
class KtWindow
{
public:
	KtWindow();

	void Cleanup();

	void Init();

	void MainLoop();

	GLFWwindow* GetGLFWWindow() const;
	KtContext& GetContext();

	void SwapBuffers() const;

	const glm::uvec2& GetSize() const;

	void SetSize(const glm::uvec2& size);

private:
	GLFWwindow* _window;
	KtContext _context;

	glm::uvec2 _size;

	void InitGLFW();
	void InitVulkan();
};

