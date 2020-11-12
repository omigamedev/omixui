module;
#include <windows.h>

export module graphics;

export class Graphics
{
    using ThisClass = Graphics;
public:
    enum class API
    {
        OpenGL_33,
        OpenGL_45,
        Vulkan,
        Metal
    };
    virtual void commit() {}
};

export class Graphics_GL33 : public Graphics
{
    using ThisClass = Graphics_GL33;
    using BaseClass = Graphics;

    HGLRC m_hRC{ NULL };
public:
    bool create_context(HDC hDC)
    {
        // setup pixel format
        PIXELFORMATDESCRIPTOR pfd{};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 32;
        pfd.iLayerType = PFD_MAIN_PLANE;

        // init GL context
        int pxfmt = ChoosePixelFormat(hDC, &pfd);
        SetPixelFormat(hDC, pxfmt, &pfd);
        m_hRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, m_hRC);
        return true;
    }
};
