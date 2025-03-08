#pragma once
#include <GLFW/glfw3.h>
#include "Context.h"
#include "glm_includes.h"
class KtWindow
{
public:
	KtWindow();

	void Init(); 
	void Cleanup();

	void MainLoop();

	GLFWwindow* GetGLFWWindow() const;
	KtContext& GetContext();
	KtRenderer& GetRenderer();

	void SwapBuffers() const;

	const glm::uvec2& GetSize() const;

	void SetSize(const glm::uvec2& size);


	void OnKeySpacePressed();

private:
	GLFWwindow* _window;
	KtContext _context;
	KtRenderer _renderer;

	glm::uvec2 _size;

	void InitGLFW();
	void InitVulkan();
};

