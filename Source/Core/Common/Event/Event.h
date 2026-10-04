#pragma once
#include "Delegate.h"
#include "Set.h"
#include "types.h"
#include "hash_utils.h"

#define _DELEGATE_ARGS(Inst, Func) Inst, Func, #Func, combine(hash_ptr(static_cast<const void*>(Inst)), ce_hash_str(#Func))

#define AddListener(Inst, Func) _AddListener(_DELEGATE_ARGS(Inst, Func))
#define RemoveListener(Inst, Func) _RemoveListener(_DELEGATE_ARGS(Inst, Func))

template<typename... Args>
class UEvent final
{
private:
    using Delegate = UDelegate<Args...>;

public:
    UEvent() = default;

    template <typename Inst, typename MemFn>
        requires std::is_member_function_pointer_v<MemFn>
            && (std::is_invocable_v<MemFn, Inst*, Args...> || std::is_invocable_v<MemFn, Inst*>)
    void _AddListener(Inst* instance, MemFn function, const char* func, const size hash)
    {
        delegates_.Add(Delegate{ instance, function, func, hash });
    }

    template <typename Inst, typename MemFn>
        requires std::is_member_function_pointer_v<MemFn>
            && (std::is_invocable_v<MemFn, Inst*, Args...> || std::is_invocable_v<MemFn, Inst*>)
    void _RemoveListener(Inst* instance, MemFn function, const char* func, const size hash)
    {
        delegates_.Remove(Delegate{ instance, function, func, hash });
    }

    template <typename... CallArgs>
        requires (std::is_convertible_v<CallArgs, Args> && ...)
    void Broadcast(CallArgs&&... args) const
    {
        // Don't process delegates that are added while the event is broadcasting
        for (i64 i{ delegates_.LastIndex() }; delegates_.IsValidIndex(i); --i)
        {
            delegates_[i].Callback(std::forward<CallArgs>(args)...);
        }
    }

    void Clear()
    {
        delegates_.Clear();
    }

private:
    USet<Delegate> delegates_;
};