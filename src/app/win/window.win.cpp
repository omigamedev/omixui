#include "window.win.h"
#include "../../util/callstack.h"
#include "../../gfx/gl33/win/gl33.win.h"

using namespace omix;

static LRESULT CALLBACK Proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    // Retrieve the class instance ptr and save it into the windows data
    if (msg == WM_CREATE)
        SetWindowLongPtr(hWnd, GWLP_USERDATA,
            (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lp)->lpCreateParams);
    // Route to the instance level handler
    if (auto w = reinterpret_cast<Window_Win64*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)))
        return w->MessageHandler(msg, wp, lp);
    // Fallback to the default handler
    return DefWindowProc(hWnd, msg, wp, lp);
}

void Window_Win64::Create(int width, int height, Graphics::API api)
{
    CALL_STACK;
    WNDCLASS wc{};
    wc.hInstance = GetModuleHandle(0);
    wc.lpfnWndProc = Proc;
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
    auto g = std::make_shared<Graphics_GL33_Win>();
    g->CreateContext(m_hDC);
    m_graphics = g;
}

void Window_Win64::SwapBuffers()
{
    ::SwapBuffers(m_hDC);
}

LRESULT Window_Win64::MessageHandler(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(m_hWnd, msg, wp, lp);
}

std::shared_ptr<Window> WindowFactory::Create(int width, int height, Graphics::API api)
{
    auto w = std::make_shared<Window_Win64>();
    w->Create(width, height, api);
    return w;
}

void RenderLoop::Run()
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
        on_render.Invoke(dt);
    }
}
