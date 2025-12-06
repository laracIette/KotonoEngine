#pragma once
#include <functional>
#include <type_traits>
#include <memory>
#include <iostream>
#include "hash_utils.h"

template <typename... Args>
class KtDelegate final
{
    friend struct std::hash<KtDelegate>;

private:
    using CallbackFunction = std::function<void(Args...)>;

public:
    template <class Tinst, class Tfunc>
        requires std::is_base_of_v<Tfunc, Tinst>
    KtDelegate(Tinst* instance, void (Tfunc::* function)(Args...))
    {
        instance_ = static_cast<void*>(instance);
        functionIdentity_ = *reinterpret_cast<void**>(&function);
        callbackFunction_ = [instance, function](Args... args) { (instance->*function)(args...); };
    }

    template <class Tinst, class Tfunc>
        requires std::is_base_of_v<Tfunc, Tinst>
    KtDelegate(const Tinst* instance, void (Tfunc::* function)(Args...) const)
    {
        instance_ = const_cast<void*>(static_cast<const void*>(instance));
        functionIdentity_ = *reinterpret_cast<void**>(&function);
        callbackFunction_ = [instance, function](Args... args) { (instance->*function)(args...); };
    }

    void Callback(Args... args) const
    {
        if (instance_)
        {
            callbackFunction_(args...);
        }
    }

    void* GetInstance() const
    {
        return instance_;
    }

    bool operator==(const KtDelegate& other) const noexcept
    {
        return other.instance_ == instance_ && other.functionIdentity_ == functionIdentity_;
    }

private:
    void* instance_;
    void* functionIdentity_;

    CallbackFunction callbackFunction_;
};

template <typename... Args>
struct std::hash<KtDelegate<Args...>>
{
    size_t operator()(const KtDelegate<Args...>& delegate) const noexcept
    {
        size_t h{ 0 };
        combine(h, std::hash<void*>{}(delegate.instance_));
        combine(h, std::hash<void*>{}(delegate.functionIdentity_));
        return h;
    }
};
