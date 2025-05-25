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
        listener.Instance = static_cast<void*>(instance);
        // Store the raw function pointer as a unique identity
        listener.FunctionIdentity = *reinterpret_cast<void**>(&function);
        listener.Callback = [instance, function](Args... args) { (instance->*function)(args...); };
        listeners_.push_back(std::move(listener));
    }

    template <typename Tinst, typename Tfunc>
        requires std::is_base_of_v<Tfunc, Tinst>
    void RemoveListener(Tinst* instance, void (Tfunc::* function)(Args...))
    {
        void* functionIdentity = *reinterpret_cast<void**>(&function);

        std::erase_if(listeners_, [=](const Listener& listener)
            {
                return listener.Instance == static_cast<void*>(instance) &&
                    listener.FunctionIdentity == functionIdentity;
            }
        );
    }

    void RemoveListener(void* instance) override
    {
        std::erase_if(listeners_, [=](const Listener& listener)
            {
                return listener.Instance == instance;
            }
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
        void* FunctionIdentity;

        std::function<void(Args...)> Callback;
    };

    std::vector<Listener> listeners_;
};