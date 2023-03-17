#pragma once
#include "engine.h"

#include <GLFW/glfw3.h>

class Input
{
private:
	Engine* m_Engine = 0;
	GLFWwindow* m_Window;
	ImVec2 dragStartingPos = ImVec2(0, 0);
	float currentScale = 1.0f;
	glm::vec3 currentTranslation = glm::vec3 (0.0f, 0.0f, 0.0f);
	ImVec2 contentWindowPos = ImVec2(0, 0);
	ImVec2 contentWindowDims = ImVec2(0, 0);
public:
	Input(GLFWwindow* w) : m_Window(w) {
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetCursorPosCallback(m_Window, StaticMouseButtonCallback);
		glfwSetScrollCallback(m_Window, StaticScrollCallback);
	};
	void SetEngine(Engine* e) {
		m_Engine = e;
	}
	void UpdateWindowDims();
private:
	static void StaticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset); // use yoffset
	static void StaticMouseButtonCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, double xpos, double ypos);
};

