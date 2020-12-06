#pragma once
#include <memory>
#include <chrono>
#include <thread>

#include <Windows.h>

#include "../window.h"

namespace omix {

class Window_Win64 : public Window
{
    HWND m_hWnd{ NULL };
    HDC m_hDC{ NULL };
    SIZE m_size{ 0 };
    std::shared_ptr<Graphics> m_graphics;
public:
    virtual void Create(int width, int height, Graphics::API api);
    virtual void SwapBuffers();
    LRESULT MessageHandler(UINT msg, WPARAM wp, LPARAM lp);
};

}