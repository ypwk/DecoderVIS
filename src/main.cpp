# include "Window.h"
// Main code
int main(int, char**)
{
    Window window = Window();
    window.Create();

    while (!glfwWindowShouldClose(window.window))
    {
        window.BeginRender();
        window.EndRender();
    }
    
    window.Shutdown();
    return 0;
}
