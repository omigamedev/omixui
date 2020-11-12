import events;
import window;
import callstack;
import std.core;

#include "macros.h"

class Foo : public EventListener
{
    float m_accumulator{ 0 };
public:
    void render_frame(float dt)
    {
        m_accumulator += dt;
        std::cout << "frame " << dt << std::endl;
    }
    virtual ~Foo()
    {

    }
};

class Texture
{

};


void recu1();

void recu0()
{
    CALL_STACK;
    static thread_local bool _entered = false;
    if (_entered)
    {
        std::cout << "already set\n";
        std::cout << CallStack::dump();
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
    recu1();
    auto t = std::thread([]() {
        recu1();
    });
    t.join();

    auto w = WindowFactory::create(800, 600, Graphics::API::OpenGL_33);

    Foo foo; 
    RenderLoop loop;
    auto f = std::make_shared<Foo>();
    loop.on_render.add(*f, &Foo::render_frame);
    loop.run();

    return 0;
}
