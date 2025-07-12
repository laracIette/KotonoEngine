#pragma once
#include <functional>
#include <type_traits>
#include <memory>
#include <iostream>
#include "log.h"
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

    void Callback(Args... args) const
    {
        if (instance_)
        {
            callbackFunction_(args...);
        }
        else
        {
            KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "can't call callbackFunction_, instance_ is nullptr");
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

namespace std
{
    template <typename... Args>
    struct hash<KtDelegate<Args...>>
    {
        size_t operator()(const KtDelegate<Args...>& delegate) const noexcept
        {
            const size_t h1 = hash<void*>{}(delegate.instance_);
            const size_t h2 = hash<void*>{}(delegate.functionIdentity_);

            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}