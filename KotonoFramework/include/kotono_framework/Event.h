#pragma once
#include <vector>
#include <functional>
#include <type_traits>
#include <memory>
#include <iostream>
template<typename... Args>
class KtEvent
{
public:
    template<class Tinst, class Tfunc>
    requires std::is_base_of_v<Tfunc, Tinst>
    void AddListener(Tinst* instance, void (Tfunc::* function)(Args...))
    {
        Listener listener{};
        listener.Instance = instance;
        listener.Callback = [instance, function](Args... args) { (instance->*function)(args...); };
        _listeners.push_back(std::move(listener));
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

    void Broadcast(Args... args)
    {
        for (const Listener& listener : _listeners)
        {
            if (listener.Instance)
            {
                listener.Callback(args...);
            }
            else
            {
                std::cerr << "can't call listener.Callback(), listener.Instance is nullptr" << std::endl;
            }
        }
    }

    void ClearListeners()
    {
        _listeners = {};
    }

private:
    struct Listener
    {
        void* Instance;
        std::function<void(Args...)> Callback;
    };

    std::vector<Listener> _listeners;
};