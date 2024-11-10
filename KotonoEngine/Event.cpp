#include "Event.h"

template<typename T>
void Event::AddObserver(T* instance, void(T::* func)())
{
    _observers.emplace_back(
        [=]()
        {
            (instance->*func)();
        }
    );
}

void Event::Invoke()
{
    for (const auto& observer : _observers)
    {
        observer();
    }
}
