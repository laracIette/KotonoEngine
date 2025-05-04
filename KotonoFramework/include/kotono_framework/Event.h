#pragma once
#include <vector>
#include <functional>
#include <type_traits>
#include <memory>
#include <iostream>
#include "EventBase.h"
#include "log.h"
template<typename... Args>
class KtEvent : public KtEventBase
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

    void RemoveListener(void* instance) override
    {
        _listeners.erase(
            std::remove_if(_listeners.begin(), _listeners.end(),
                [instance](const Listener& listener)
                { 
                    return listener.Instance == instance; 
                }
            ),
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