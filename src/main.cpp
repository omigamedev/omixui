#include <iostream>
#include <thread>
#include "util/events.h"
#include "app/window.h"
#include "util/callstack.h"

using namespace omix;

void recu1();

void recu0()
{
    CALL_STACK;
    static thread_local bool _entered = false;
    if (_entered)
    {
        std::cout << "already set\n";
        std::cout << CallStack::Dump();
        return;
    }
    LoopGuard _loop_guard(_entered);
    std::cout << "recu0\n";
    recu1();
}

void recu1()
{
    CALL_STACK;
    std::cout << "recu1\n";
    recu0();
}

int main()
{
    CALL_STACK;
    auto wgl = WindowFactory::Create(800, 600, Graphics::API::OpenGL_33);
    auto wdx = WindowFactory::Create(800, 600, Graphics::API::Direct3D_11);

    RenderLoop loop;
    loop.on_render.Add([](float) { std::cout << "lambda dt\n"; });
    loop.Run();

    return 0;
}
