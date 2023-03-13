#include "Controller.h"

Controller::Controller()
{
}

bool Controller::render()
{
    ImGui::Begin("Properties");
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Position");
        ImGui::SameLine(0, 5.0f);
    }

    if (ImGui::CollapsingHeader("Light"))
    {

        ImGui::Separator();
        ImGui::Text("Position");
        ImGui::Separator();
    }

    ImGui::End();
	return false;
}
