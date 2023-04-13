#pragma once
#include "engine.h"

#include <GLFW/glfw3.h>

class Input
{
private:
	Engine* m_Engine = 0;

	
	float sens = 0.5;

	bool isDragging = false;

	glm::vec3 currentTranslation = glm::vec3 (0.0f, 0.0f, 0.0f);
	
public:
	float currentScale = 0.0f;
	float scaleQuant = 1.1f;

	Input(GLFWwindow* w){
		glfwSetWindowUserPointer(w, this);
		glfwSetScrollCallback(w, StaticScrollCallback);
	};
	void SetEngine(Engine* e) {
		m_Engine = e;
	}
	void MouseInput();

private:
	static void StaticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset); // use yoffset
	
};

