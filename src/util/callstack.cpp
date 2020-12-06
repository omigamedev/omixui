#include "callstack.h"
#include <map>
#include <thread>
#include <sstream>
#include <deque>

using namespace omix;

static std::map<std::thread::id, std::deque<std::pair<std::string, int>>> s_stacks;
static thread_local int s_depth;

CallStack::CallStack(const std::string& name)
{
    auto tid = std::this_thread::get_id();
    s_stacks[tid].emplace_back(name, s_depth++);
}

CallStack::~CallStack()
{
    auto tid = std::this_thread::get_id();
    s_stacks[tid].pop_back();
    s_depth--;
}

std::string CallStack::Dump()
{
    std::ostringstream ss;
    for (auto& [tid, stack] : s_stacks)
    {
        ss << "thread-" << tid << '\n';
        for (auto& [s, n] : stack)
            ss << std::string(n, ' ') << s << std::endl;
    }
    return ss.str();
}
