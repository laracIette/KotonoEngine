#pragma once
#include <vector>
#include <functional>
class Event final
{
public:
    template <typename T>
    void Add(T* object, void (T::* memberFunction)());

    template <typename T>
    void Remove(T* object, void (T::* memberFunction)());

    // Triggers the event and calls all observers
    void Invoke();

private:
    struct Observer
    {
        void* Object;
        void* MemberFunction;

        std::function<void()> Callback;
    };

    std::vector<Observer> _observers;
};

