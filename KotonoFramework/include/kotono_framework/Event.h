#pragma once
#include "Delegate.h"
#include "Pool.h"

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
        delegates_.RemoveIf([](const Delegate& delegate) { return !delegate.GetInstance(); });
        // Don't process delegates that are added while the event is broadcasting
        for (int64_t i{ delegates_.LastIndex()}; i >= 0; --i)
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