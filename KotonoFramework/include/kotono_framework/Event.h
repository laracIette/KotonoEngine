#pragma once
#include <vector>
#include <functional>
#include <algorithm>

class KtEvent
{
public:
    template<class Tinst, class Tfunc, typename... Args >
    requires std::is_base_of_v<Tfunc, Tinst>
    void AddListener(Tinst* instance, void (Tfunc::* function)(Args...), Args... args)
    {
        _listeners.push_back({
            instance,
            [instance, function, args...]() { (instance->*function)(args...); }
        });
    }

    template<class Tinst, class Tfunc, typename... Args >
    requires std::is_base_of_v<Tfunc, Tinst>
    void AddListener(Tinst* instance, void (Tfunc::* function)(Args...) const, Args... args)
    {
        _listeners.push_back({
            instance,
            [instance, function, args...]() { (instance->*function)(args...); }
        });
    }

    template<class T>
    void RemoveListener(T* instance)
    {
        _listeners.erase(
            std::remove_if(_listeners.begin(), _listeners.end(),
                [instance](const Listener& listener) { return listener.Instance == instance; }),
            _listeners.end()
        );
    }

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

