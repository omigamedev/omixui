#pragma once
#include <memory>

#include "../gfx/graphics.h"
#include "../util/events.h"

namespace omix {

class Window : std::enable_shared_from_this<Window>
{
public:
    virtual void Create(int width, int height, Graphics::API api) = 0;
    virtual void SwapBuffers() = 0;
};

class WindowFactory
{
public:
    static std::shared_ptr<Window> Create(int width, int height, Graphics::API api);
};

class RenderLoop
{
public:
    Event<void(float)> on_render;
    virtual void Run();
};

}
