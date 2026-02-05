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

    void Broadcast(Args... args)
    {
        // Don't process delegates that are added while the event is broadcasting
        for (i64 i{ delegates_.LastIndex() }; i >= 0; --i)
        {
            delegates_[i].Callback(args...);
        }
    }

    void Clear()
    {
        delegates_.Clear();
    }

private:
    KtPool<Delegate> delegates_;
};