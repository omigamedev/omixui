export module callstack;
import std.core;

export class CallStack
{
    static inline std::map<std::thread::id, std::deque<std::pair<std::string, int>>> m_stacks;
    static inline thread_local int m_depth;
public:
    CallStack(const std::string& name) 
    {
        auto tid = std::this_thread::get_id();
        m_stacks[tid].emplace_back(name, m_depth++);
    }
    ~CallStack()
    {
        auto tid = std::this_thread::get_id();
        m_stacks[tid].pop_back();
        m_depth--;
    }
    static std::string dump()
    {
        std::ostringstream ss;
        for (auto& [tid, stack] : m_stacks)
        {
            ss << "thread-" << tid << '\n';
            for (auto& [s, n] : stack)
                ss << std::string(n, ' ') << s << std::endl;
        }
        return ss.str();
    }
};

export struct LoopGuard
{
    bool& value;
    LoopGuard(bool& static_ref) : value(static_ref) { value = true; }
    ~LoopGuard() { value = false; }
};
