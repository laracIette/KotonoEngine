#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window final
{
public:
	Window();
	~Window();

	void Init();
	void MainLoop();

private:
	GLFWwindow* _window;

	glm::uvec2 _windowSize;
};

