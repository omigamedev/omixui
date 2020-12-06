#pragma once
#include <string>

namespace omix {

class CallStack
{
public:
    CallStack(const std::string& name);
    ~CallStack();
    static std::string Dump();
};

struct LoopGuard
{
    bool& value;
    inline LoopGuard(bool& static_ref) : value(static_ref) { value = true; }
    inline ~LoopGuard() { value = false; }
};

}
