#pragma once
#include <functional>
#include <string_view>
#include <type_traits>
#include "hash_utils.h"
#include "types.h"

#define _MAKE_DELEGATE(Inst, Func) UDelegate(Inst, Func, #Func, combine(hash_ptr(static_cast<const void*>(Inst)), ce_hash_str(#Func)))

template <typename... Args>
class UDelegate final
{
    friend struct std::hash<UDelegate>;

private:
    using CallbackFunction = std::function<void(Args...)>;

public:
    template <typename Tinst, typename Tfunc>
        requires std::is_base_of_v<Tfunc, Tinst>
    UDelegate(Tinst* instance, void (Tfunc::* function)(Args...), const char* func, const size hash)
    {
        callbackFunction_ = [instance, function](Args... args) { (instance->*function)(std::forward<Args>(args)...); };
        inst_ = instance;
        func_ = func;
        hash_ = hash;
    }

    template <typename Tinst, typename Tfunc>
        requires std::is_base_of_v<Tfunc, Tinst>
    UDelegate(const Tinst* instance, void (Tfunc::* function)(Args...) const, const char* func, const size hash)
    {
        callbackFunction_ = [instance, function](Args... args) { (instance->*function)(std::forward<Args>(args)...); };
        inst_ = instance;
        func_ = func;
        hash_ = hash;
    }

    template <typename... CallArgs>
        requires (std::is_convertible_v<CallArgs, Args> && ...)
    void Callback(CallArgs&&... args) const
    {
        callbackFunction_(std::forward<CallArgs>(args)...);
    }

    bool operator==(const UDelegate& other) const noexcept
    {
        return other.inst_ == inst_
            && std::string_view{ other.func_ } == std::string_view{ func_ };
    }

private:
    CallbackFunction callbackFunction_;
    const void* inst_;
    const char* func_;
    size hash_;
};

template <typename... Args>
struct std::hash<UDelegate<Args...>>
{
    ::size operator()(const UDelegate<Args...>& delegate) const noexcept
    {
        return delegate.hash_;
    }
};
