#pragma once
#include <functional>
#include <type_traits>
#include <memory>
#include <iostream>
template<typename... Args>
struct KtDelegate
{
public:
    typedef std::function<void(Args...)> CallbackFunction;
    
    template<class Tinst, class Tfunc>
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
            std::cerr << "can't call callbackFunction_, instance_ is nullptr" << std::endl;
        }
    }

    const bool GetIsSameInstance(void* instance) const noexcept
    {
        return instance == instance_;
    }

    const bool operator==(const KtDelegate& other) const noexcept
    {
        return other.instance_ == instance_ && other.functionIdentity_ == functionIdentity_;
    }

    friend struct std::hash<KtDelegate>;

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
        std::size_t operator()(const KtDelegate<Args...>& delegate) const noexcept
        {
            std::size_t h1 = std::hash<void*>{}(delegate.instance_);
            std::size_t h2 = std::hash<void*>{}(delegate.functionIdentity_);

            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}