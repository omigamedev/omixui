#pragma once
#include <vector>

namespace omix {

#define LOOP_GUARD \
    static thread_local bool _loop_guard = false;\
    if (_loop_guard) throw std::runtime_error("LoopGuard failed");\
    loop_guard_t loop_guard(_loop_guard);

#define CALL_STACK \
    CallStack _callstack(__FUNCTION__);

class EventBase
{
public:
    virtual void Remove(class EventListener* obj) = 0;
};

class EventListener
{
    template<typename Sig> friend class Event;
protected:
    std::vector<EventBase*> m_events;
    inline virtual ~EventListener()
    {
        for (auto& e : m_events)
            e->Remove(this);
    }
};

template<typename Sig> class Event;
template<typename... Args>
class Event<void(Args...)> : public EventBase
{
    using Method = void(EventListener::*)(Args...);
    using Callback = void(*)(Args...);
    std::vector<std::pair<EventListener*, Method>> m_listeners;
    std::vector<Callback> m_callbacks;
public:
    template<typename T, typename F>
    inline void Add(T& obj, F method)
    {
        obj.m_events.emplace_back(this);
        m_listeners.emplace_back(&obj, static_cast<Method>(method));
    }
    template<typename F>
    inline void Add(F fn)
    {
        // C++20 version
        //struct Lambda : public F, public EventListener
        //{
        //    inline void run(Args... args) { operator()(args...); }
        //};
        //static Lambda obj;
        struct Lambda : public EventListener
        {
            const F fn;
            Lambda(F fn) : fn(fn) {}
            inline void run(Args... args) { fn(args...); }
        };
        static Lambda obj(fn);
        Add(obj, &Lambda::run);
    }
    inline void Add(Callback fn)
    {
        m_callbacks.emplace_back(fn);
    }
    virtual void Remove(EventListener* obj) override
    {
        // C++20 version
        //std::erase_if(m_listeners,
        //    [obj](auto& item) { return item.first == obj; }));
        m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(),
            [obj](auto& item) { return item.first == obj; }));
    }
    void Invoke(Args... args)
    {
        for (auto& [obj, fn] : m_listeners)
            (obj->*fn)(args...);
        for (auto& fn : m_callbacks)
            fn(args...);
    }
    virtual ~Event()
    {
        // Remove this event from every listener
        for (auto& [obj, fn] : m_listeners)
        {
            // C++20 version
            //std::erase_if(obj->m_events, [this](auto& item) {
            //    return item == this;
            //});
            obj->m_events.erase(std::remove_if(obj->m_events.begin(), obj->m_events.end(),
                [this](auto& item) { return item == this; }));
        }
    }
};

}
