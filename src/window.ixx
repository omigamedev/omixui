module;
#include <windows.h>
#include "macros.h"

export module window;
import graphics;
import events;
import std.core;

export class Window : std::enable_shared_from_this<Window>
{
    using ThisClass = Window;
public:
    virtual void create(int width, int height, Graphics::API api) = 0;
    virtual void swap_buffers() = 0;
};

export class Window_Win32 : public Window
{
    using ThisClass = Window_Win32;
    using BaseClass = Window;

    static LRESULT CALLBACK proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        // Retrieve the class instance ptr and save it into the windows data
        if (msg == WM_CREATE) [[unlikely]]
            SetWindowLongPtr(hWnd, GWLP_USERDATA,
                (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lp)->lpCreateParams);
        // Route to the instance level handler
        if (auto w = reinterpret_cast<ThisClass*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))) [[likely]]
            return w->message_handler(msg, wp, lp);
        // Fallback to the default handler
        return DefWindowProc(hWnd, msg, wp, lp);
    }

    HWND m_hWnd{ NULL };
    HDC m_hDC{ NULL };
    SIZE m_size{ 0 };
    std::shared_ptr<Graphics> m_graphics;

public:

    virtual void create(int width, int height, Graphics::API api)
    {
        CALL_STACK;
        WNDCLASS wc{};
        wc.hInstance = GetModuleHandle(0);
        wc.lpfnWndProc = proc;
        wc.lpszClassName = TEXT("MainWindow");
        // Extra space to store "this" ptr
        wc.cbWndExtra = sizeof(Window*);
        RegisterClass(&wc);
        RECT r = { 0, 0, width, height };
        AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
        m_size = { r.right - r.left, r.bottom - r.top };
        m_hWnd = CreateWindow(wc.lpszClassName, TEXT("MainWindow"), WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, m_size.cx, m_size.cy,
            NULL, NULL, wc.hInstance, this);
        m_hDC = GetDC(m_hWnd);
        ShowWindow(m_hWnd, SW_NORMAL);

        // Create graphics context
        // Only OpenGL33 at the moment
        auto g = std::make_shared<Graphics_GL33>();
        g->create_context(m_hDC);
        m_graphics = g;
    }

    virtual void swap_buffers()
    {
        SwapBuffers(m_hDC);
    }

    LRESULT message_handler(UINT msg, WPARAM wp, LPARAM lp)
    {
        switch (msg)
        {
        [[unlikely]] case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(m_hWnd, msg, wp, lp);
    }
};

export class WindowFactory
{
public:
    static std::shared_ptr<Window> create(int width, int height, Graphics::API api)
    {
        auto w = std::make_shared<Window_Win32>();
        w->create(width, height, api);
        return w;
    }
};

export class RenderLoop
{
public:
    Event<void(float)> on_render;
    virtual void run()
    {
        using clock = std::chrono::high_resolution_clock;
        using seconds = std::chrono::duration<float>;
        MSG msg;
        auto start = clock::now();
        while (GetMessage(&msg, 0, 0, 0))
        {
            auto now = clock::now();
            float dt = std::chrono::duration_cast<seconds>(now - start).count();
            start = now;
            DispatchMessage(&msg);
            on_render.invoke(dt);
        }
    }
};
