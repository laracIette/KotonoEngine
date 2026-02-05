#pragma once
#include <functional>
#include <type_traits>
#include <memory>
#include <iostream>
#include "hash_utils.h"
#include "types.h"

#define _MAKE_DELEGATE(Inst, Func) UDelegate(Inst, Func, combine(hash_ptr(static_cast<void*>(Inst)), ce_hash_str(#Func)))

template <typename... Args>
class UDelegate final
{
    friend struct std::hash<UDelegate>;

private:
    using CallbackFunction = std::function<void(Args...)>;

public:
    template <class Tinst, class Tfunc>
        requires std::is_base_of_v<Tfunc, Tinst>
    UDelegate(Tinst* instance, void (Tfunc::* function)(Args...), const size hash)
    {
        hash_ = hash;
        callbackFunction_ = [instance, function](Args... args) { (instance->*function)(args...); };
    }

    template <class Tinst, class Tfunc>
        requires std::is_base_of_v<Tfunc, Tinst>
    UDelegate(const Tinst* instance, void (Tfunc::* function)(Args...) const, const size hash)
    {
        hash_ = hash;
        callbackFunction_ = [instance, function](Args... args) { (instance->*function)(args...); };
    }

    void Callback(Args... args) const
    {
        callbackFunction_(args...);
    }

    bool operator==(const UDelegate& other) const noexcept
    {
        return other.hash_ == hash_;
    }

private:
    size hash_;
    CallbackFunction callbackFunction_;
};

template <typename... Args>
struct std::hash<UDelegate<Args...>>
{
    ::size operator()(const UDelegate<Args...>& delegate) const noexcept
    {
        return delegate.hash_;
    }
};
