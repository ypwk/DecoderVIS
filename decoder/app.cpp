#include <stdio.h>
#include <iostream>

#include "ig_window.h"
#include "ig_contentwindow.h"
#include "engine.h"
#include "RotatedPlanarCode.h"
#include "central_node_handler.h"

#include <GLFW\glfw3.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(void)
{
    glfwSetErrorCallback(glfw_error_callback);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        ImGui_ContentWindowHandler IG_CWHandler(1280, 720);
        IG_CWHandler.RenderInit();

        // init engine
        Engine RenderEngine = Engine();

        // init central node handler
        CentralNodeHandler CNH = CentralNodeHandler();

        // init surface code
        GenericCode* currentCode = new RotatedPlanarCode(3);

        ImGui_Handler IG_Handler = ImGui_Handler();
        IG_Handler.Create(window);

        glm::vec3 translationa(100, 100, 0);
        glm::vec3 translationb(500, 500, 0);

        float size = 400.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            // clear
            IG_Handler.NewFrame();

            // render central node
            CNH.Render();

            // render content window
            IG_CWHandler.PreRender();

            // render content in content window
            RenderEngine.Clear();

            ImGuiIO& io = ImGui::GetIO(); (void)io;
            float ratio = ImGui::GetWindowWidth() / ImGui::GetWindowHeight() * io.DisplaySize.y /  io.DisplaySize.x;
            
            //RenderEngine.AddCircle(translationa, 50, ratio, glm::vec4(0.0f, 255.0f, 0.0f, 1.0f));

            RenderEngine.AddCircle(translationb, size, ratio, glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));

            //RenderEngine.AddCircle(translationb, 50, ratio, glm::vec4(202.0f / 256, 85.0f / 256, 85.0f / 256, 1.0f));

            RenderEngine.AddQubit(translationa, ratio, glm::vec4(202.0f / 256, 85.0f / 256, 85.0f / 256, 1.0f));

            

            RenderEngine.Render();
            // end render content in content window 

            IG_CWHandler.PostRender();

            // render controller window
            ImGui::Begin("Controller");
            {
                ImGuiIO& io = ImGui::GetIO(); (void)io;

                ImGui::Text("DisplaySize = %f,%f", io.DisplaySize.x, io.DisplaySize.y);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

                ImGui::SliderFloat3("translationa", &translationa.x, 0.0f, 960.0f);

                ImGui::SliderFloat3("translationb", &translationb.x, 0.0f, 960.0f);

                ImGui::SliderFloat("raidus", &size, 0.0f, 1000.0f);
            }
            ImGui::End();

            IG_Handler.Render();
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Cleanup
        IG_Handler.Shutdown();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}