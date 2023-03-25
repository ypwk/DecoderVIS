#include "input.h"


void Input::StaticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input* that = static_cast<Input*>(glfwGetWindowUserPointer(window));
	that->ScrollCallback(window, xoffset, yoffset);
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	currentScale += (float) yoffset;
	m_Engine->UpdateView(pow(scaleQuant, currentScale), m_Engine->GetTranslation());

}

void Input::MouseInput()
{
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 mousePos = ImGui::GetMousePos();
	ImVec2 windowDim = ImGui::GetContentRegionAvail();

	if (ImGui::IsMouseDragging(0) && mousePos.x > windowPos.x \
		&& mousePos.x < windowPos.x + windowDim.x && mousePos.y > windowPos.y && \
		mousePos.y < windowPos.y + windowDim.y) {
		if (!isDragging) {
			isDragging = true;
			currentTranslation = m_Engine->GetTranslation();
		}
		ImVec2 del = ImGui::GetMouseDragDelta();
		currentTranslation = currentTranslation - glm::vec3(- del.x / pow(scaleQuant, currentScale), \
			del.y / pow(scaleQuant, currentScale), 0);
		m_Engine->UpdateView(pow(scaleQuant, currentScale), currentTranslation);
	}
	isDragging = false;
}
