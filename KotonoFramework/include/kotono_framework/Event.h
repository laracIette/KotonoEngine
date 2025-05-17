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
        listeners_.push_back(std::move(listener));
    }

    void RemoveListener(void* instance) override
    {
        listeners_.erase(
            std::remove_if(listeners_.begin(), listeners_.end(),
                [instance](const Listener& listener)
                { 
                    return listener.Instance == instance; 
                }
            ),
            listeners_.end()
        );
    }

    void Broadcast(Args... args)
    {
        for (const Listener& listener : listeners_)
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
        listeners_ = {};
    }

private:
    struct Listener
    {
        void* Instance;
        std::function<void(Args...)> Callback;
    };

    std::vector<Listener> listeners_;
};