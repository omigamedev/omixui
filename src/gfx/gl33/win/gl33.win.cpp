#include "gl33.win.h"

using namespace omix;

bool Graphics_GL33_Win::CreateContext(HDC hDC)
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

Texture::Ref Graphics_GL33_Win::CreateTexture()
{
    auto t = std::make_shared<Texture_GL33_Win>();
    glGenTextures(1, &t->m_texture_id);
    return t;
}
