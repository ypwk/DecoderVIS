#include <stdio.h>
#include <iostream>

#include "ig_window.h"
#include "renderer.h"

#include "vertex_buffer.h"
#include "index_buffer.h"

#include <GLFW\glfw3.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>



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

    ImGui_Handler IG_Handler = ImGui_Handler();
    IG_Handler.Create(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // render clear
        glClear(GL_COLOR_BUFFER_BIT);

        IG_Handler.NewFrame();

        // shader bind
        
        // render draw

        IG_Handler.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    IG_Handler.Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}