#pragma once
#include <vector>
#include <functional>
#include <algorithm>

class Event
{
public:
    template<class T, typename... Args>
    void AddListener(T* instance, void (T::* function)(Args...), Args... args);
    template<class T>
    void RemoveListener(T* instance);
    void Broadcast();

private:
    struct Listener
    {
        void* Instance;
        std::function<void()> Callback;

        bool operator==(const Listener& other) const
        {
            return Instance == other.Instance;
        }
    };

    std::vector<Listener> _listeners;
};

