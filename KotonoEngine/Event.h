#pragma once
#include <vector>
#include <functional>
class Event final
{
public:
    // Adds an observer with a callable function
    template <typename T>
    void AddObserver(T* instance, void (T::* func)());

    // Triggers the event and calls all _observers
    void Invoke();

private:
    std::vector<std::function<void()>> _observers;
};