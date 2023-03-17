#include "input.h"

void Input::UpdateWindowDims()
{
	contentWindowDims = ImGui::GetContentRegionAvail();
	contentWindowPos = ImGui::GetWindowPos();
}

void Input::StaticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input* that = static_cast<Input*>(glfwGetWindowUserPointer(window));
	that->ScrollCallback(window, xoffset, yoffset);
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	currentScale += (float) yoffset;
	m_Engine->UpdateView(pow(1.5f, currentScale), currentTranslation);

}

void Input::StaticMouseButtonCallback(GLFWwindow* window, double xpos, double ypos)
{
	Input* that = static_cast<Input*>(glfwGetWindowUserPointer(window));
	that->MouseButtonCallback(window, xpos, ypos);
}

void Input::MouseButtonCallback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << xpos << " " << ypos << " " << contentWindowPos.x << " " << contentWindowDims.x << std::endl;
	if (xpos < contentWindowDims.x && xpos > contentWindowPos.x \
		&& ypos < contentWindowPos.y + contentWindowDims.y && ypos > contentWindowPos.y) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			dragStartingPos = ImVec2(-1, -1);
			return;
		}

		if (dragStartingPos[0] == -1 && dragStartingPos[1] == -1) {
			currentTranslation = m_Engine->GetView();
			dragStartingPos = ImVec2(currentTranslation.x, currentTranslation.y);
		}

		glm::vec3 copyTranslation = currentTranslation + glm::vec3((float)xpos - dragStartingPos.x, (float)ypos - dragStartingPos.y, 0);
		m_Engine->UpdateView(currentScale, copyTranslation);
	}
}
