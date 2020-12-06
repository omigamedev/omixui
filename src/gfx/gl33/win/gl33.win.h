#pragma once
#include "../gl33.h"
#include <gl/GL.h>

namespace omix {

class Texture_GL33_Win : public Texture_GL33
{
    friend class Graphics_GL33_Win;
public:
};


class Graphics_GL33_Win : public Graphics_GL33
{
    HGLRC m_hRC{ NULL };

public:
    bool CreateContext(HDC hDC);
    Texture::Ref CreateTexture() override;
};

}
