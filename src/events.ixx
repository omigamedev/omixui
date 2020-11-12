export module events;
import std.core;

export class EventBase
{
public:
    virtual void remove(class EventListener* obj) = 0;
};

export class EventListener
{
    template<typename Sig> friend class Event;
protected:
    std::vector<EventBase*> m_events;
    virtual ~EventListener()
    {
        for (auto& e : m_events)
            e->remove(this);
    }
};

export template<typename Sig> class Event;
export template<typename... Args>
class Event<void(Args...)> : public EventBase
{
    using Callback = void(EventListener::*)(Args...);
    std::vector<std::pair<EventListener*, Callback>> m_listeners;
public:
    template<typename T, typename F>
    void add(T& obj, F method)
    {
        obj.m_events.emplace_back(this);
        m_listeners.emplace_back(&obj, static_cast<Callback>(method));
    }
    virtual void remove(EventListener* obj) override
    {
        std::erase_if(m_listeners,
            [obj](auto& item) { return item.first == obj; });
    }
    void invoke(Args... args)
    {
        for (auto& [obj, fn] : m_listeners)
            (obj->*fn)(args...);
    }
    virtual ~Event()
    {
        // Remove this event from every listener
        for (auto& [obj, fn] : m_listeners)
        {
            std::erase_if(obj->m_events, [this](auto& item) {
                return item == this;
            });
        }
    }
};
