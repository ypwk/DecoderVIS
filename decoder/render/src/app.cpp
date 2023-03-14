#include <stdio.h>
#include <iostream>

#include "ig_window.h"
#include "renderer.h"

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "ig_contentwindow.h"

#include <GLFW\glfw3.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>
#include <texture.h>

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
        

        float positions[] = {
             50.0f,  50.0f, 1.0f,  1.0f,// 0
             -50.0f,  50.0f, 0.0f,  1.0f,// 1
             -50.0f,  -50.0f, 0.0f,  0.0f,// 2
             50.0f,  -50.0f, 1.0f,  0.0f,// 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        
        ImGui_ContentWindowHandler IG_CWHandler(1280, 720);
        IG_CWHandler.RenderInit();

        // set up renderer rendering
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::vec3 translationa(0, 0, 0);
        glm::vec3 translationb(0, 0, 0);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationa);

        glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/Texture.shader");
        //Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/iced_chris.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui_Handler IG_Handler = ImGui_Handler();
        IG_Handler.Create(window);

        float r = 0.0f;
        float increment = 0.05f;

        const int W = 1080;
        const int H = 1920;

        ImGui::SetNextWindowSize(ImVec2(W + 10, H + 10));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            IG_Handler.NewFrame();

            // render content window
            IG_CWHandler.PreRender();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationa);
                glm::mat4 mvp = proj * view * model;

                // shader bind
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                // render draw
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationb);
                glm::mat4 mvp = proj * view * model;

                // shader bind
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                // render draw
                renderer.Draw(va, ib, shader);
            }

            IG_CWHandler.PostRender();
                

            ImGui::Begin("Controller");
            {
                ImGuiIO& io = ImGui::GetIO(); (void)io;

                ImGui::Text("DisplaySize = %f,%f", io.DisplaySize.x, io.DisplaySize.y);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

                ImGui::SliderFloat3("translationa", &translationa.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("translationb", &translationb.x, 0.0f, 960.0f);
            }
            ImGui::End();

            

            IG_Handler.Render();
            

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        // Cleanup
        IG_Handler.Shutdown();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}