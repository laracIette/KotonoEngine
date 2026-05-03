#pragma once
#include "Delegate.h"
#include "Pool.h"
#include "types.h"
#include "hash_utils.h"

#define AddListener(Inst, Func) _AddListener(_MAKE_DELEGATE(Inst, Func))
#define RemoveListener(Inst, Func) _RemoveListener(_MAKE_DELEGATE(Inst, Func))

template<typename... Args>
class UEvent final
{
private:
    using Delegate = UDelegate<Args...>;

public:
    void _AddListener(Delegate&& delegate)
    {
        delegates_.Add(std::move(delegate));
    }

    void _RemoveListener(const Delegate& delegate)
    {
        delegates_.Remove(delegate);
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
    UPool<Delegate> delegates_;
};