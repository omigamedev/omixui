#pragma once

#define LOOP_GUARD \
    static thread_local bool _loop_guard = false;\
    if (_loop_guard) throw std::runtime_error("LoopGuard failed");\
    loop_guard_t loop_guard(_loop_guard);

#define CALL_STACK \
    CallStack _callstack(__FUNCTION__);
