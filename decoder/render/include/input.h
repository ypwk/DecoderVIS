#pragma once
#include "engine.h"

#include <GLFW/glfw3.h>

class Input
{
private:
	Engine* m_Engine = 0;
	GLFWwindow* m_Window;
	float currentScale = 0.0f;
	float scaleQuant = 1.1f;
	glm::vec3 currentTranslation = glm::vec3 (0.0f, 0.0f, 0.0f);
	bool isDragging = false;
	float sens = 0.5;
public:
	Input(GLFWwindow* w) : m_Window(w) {
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetScrollCallback(m_Window, StaticScrollCallback);
	};
	void SetEngine(Engine* e) {
		m_Engine = e;
	}
	void MouseInput();
private:
	static void StaticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset); // use yoffset
	
};

