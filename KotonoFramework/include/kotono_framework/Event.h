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
        //KT_LOG_KF(KT_LOG_COMPILE_TIME_LEVEL, "%llu delegates", delegates_.Size());
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