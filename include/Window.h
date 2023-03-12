#pragma once


class Window
{
public:
    Window() {};
    ~Window();

    bool Create();
    void Shutdown();

    void PumpEvents();

    void GetSize(int& w, int& h);

    void BeginRender();
    void EndRender();

private:
};

