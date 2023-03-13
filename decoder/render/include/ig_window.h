#pragma once

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class ImGui_Handler
{
public:
    ImGui_Handler() {};
    ~ImGui_Handler() {};

    void Create(GLFWwindow* window);
    void NewFrame();
    void Render();

    void Shutdown();
};
    

