#include "App.h"

App::App()
{
    current_code = RotatedPlanarCode(3);
    controller = Controller();
    scene = SceneView();
}

void App::run()
{
    window = Window();

    window.Create();

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!window.shouldClose())
#endif
    {
        window.PreRender();

        // render controller
        controller.render();

        // render surface code visualization
        
        // scene.render(current_code);

        window.PostRender();

        handle_input();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    window.Shutdown();
}

void App::handle_input()
{
    /*double x, y;
    glfwGetCursorPos(window.window, &x, &y);
    printf("%d %d %f", x, y, window.clear_color.x);*/
}

