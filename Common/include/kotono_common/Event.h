#pragma once
#include "Delegate.h"
#include "Pool.h"
#include "types.h"

template<typename... Args>
class KtEvent final
{
private:
    using Delegate = KtDelegate<Args...>;

public:
    void AddListener(const Delegate& delegate)
    {
        delegates_.Add(delegate);
    }

    void AddListener(Delegate&& delegate)
    {
        delegates_.Add(std::move(delegate));
    }

    void RemoveListener(const Delegate& delegate)
    {
        delegates_.Remove(delegate);
    }

    void Broadcast(Args... args)
    {
        // Don't process delegates that are added while the event is broadcasting
        for (i64 i{ delegates_.LastIndex()}; i >= 0; --i)
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