#include "Event.h"

template<typename T, typename ...Args>
void Event::AddListener(T* instance, void(T::* function)(Args...), Args... args)
{
    _listeners.push_back({
        instance,
        [instance, function, args...]() { (instance->*function)(args...); }
    });
}

template<typename T>
void Event::RemoveListener(T* instance)
{
    _listeners.erase(
        std::remove_if(_listeners.begin(), _listeners.end(),
            [instance](const Listener& listener) { return listener.Instance == instance; }),
        _listeners.end()
    );
}

void Event::Broadcast()
{
    for (const Listener& listener : _listeners)
    {
        listener.Callback();
    }
}
