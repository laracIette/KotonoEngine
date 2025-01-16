#include "Event.h"

template <typename T>
void Event::Add(T* object, void (T::* memberFunction)())
{
    _observers.emplace_back(
        static_cast<void*>(object),
        static_cast<void*>(memberFunction),
        [object, memberFunction]()
        {
            if (object)
            {
                (object->*memberFunction)();
            }
        }
    );
}

template <typename T>
void Event::Remove(T* object, void (T::* memberFunction)())
{
    auto it = std::remove_if(
        _observers.begin(),
        _observers.end(),
        [object, memberFunction](const Observer& observer)
        {
            return object == observer.Object && memberFunction == object.MemberFunction;
        }
    );
    _observers.erase(it, _observers.end());
}

void Event::Invoke()
{
    for (const auto& observer : _observers)
    {
        observer.Callback();
    }
}