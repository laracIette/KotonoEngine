#pragma once
#include <functional>
#include <string_view>
#include <type_traits>
#include "hash_utils.h"
#include "types.h"

template <typename... Args>
class UDelegate final
{
    friend struct std::hash<UDelegate>;

private:
    using CallbackFunction = std::function<void(Args...)>;

public:
    UDelegate(const void* inst, const char* func, size hash, CallbackFunction callbackFunction)
        : inst_{ inst }
        , func_{ func }
        , hash_{ hash }
        , callbackFunction_{ callbackFunction }
    {}

    template <typename Inst, typename MemFn>
        requires std::is_member_function_pointer_v<MemFn>
            && (std::is_invocable_v<MemFn, Inst*, Args...> || std::is_invocable_v<MemFn, Inst*>)
    UDelegate(Inst* instance, MemFn function, const char* func, const size hash)
        : UDelegate{ instance, func, hash
        , [instance, function](Args... args) {
            if constexpr (std::is_invocable_v<MemFn, Inst*, Args...>)
            {
                std::invoke(function, instance, std::forward<Args>(args)...);
            }
            else if constexpr (std::is_invocable_v<MemFn, Inst*>)
            {
                std::invoke(function, instance);
            }
        } }
    {}

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
    const void* inst_;
    const char* func_;
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
