#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ObjectManager.h"

class Window final
{
public:
	Window();
	~Window();

	void Init();
	void MainLoop();

	ObjectManager* GetObjectManager() const;

private:
	GLFWwindow* _window;

	glm::uvec2 _windowSize;

	ObjectManager* _objectManager;
};

