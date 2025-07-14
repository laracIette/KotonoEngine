#pragma once
#include "Delegate.h"
#include "Pool.h"

#define KT_LOG_IMPORTANCE_LEVEL_EVENT KT_LOG_IMPORTANCE_LEVEL_NONE

const int64_t incrementEventInstances();
const int64_t decrementEventInstances();

template<typename... Args>
class KtEvent final
{
private:
    using Delegate = KtDelegate<Args...>;

public:
    KtEvent()
    { 
        KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_EVENT, "inc %lld events", incrementEventInstances());
    }
    ~KtEvent()
    { 
        KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_EVENT, "dec %lld events", decrementEventInstances());
    }

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
        for (const Delegate& delegate : delegates_)
        {
            delegate.Callback(args...);
        }
    }

private:
    KtPool<Delegate> delegates_;
};