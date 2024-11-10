#pragma once
#include <GL/glew.h>  // Use GLEW for managing OpenGL extensions
#include <GLFW/glfw3.h>

class Window final
{
public:
	Window();
	~Window();

	void Init();
	void MainLoop();

private:
	GLFWwindow* _window;
};

