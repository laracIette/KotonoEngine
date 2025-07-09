#pragma once
#include <unordered_set>
#include "EventBase.h"
#include "Delegate.h"
template<typename... Args>
class KtEvent final : public KtEventBase
{
public:
    void AddListener(const KtDelegate<Args...>& delegate)
    {
        delegates_.insert(delegate);
    }
    
    void AddListener(KtDelegate<Args...>&& delegate)
    {
        delegates_.insert(std::move(delegate));
    }

    void RemoveListener(const KtDelegate<Args...>& delegate)
    {
        delegates_.erase(delegate);
    }

    void RemoveListener(void* instance) override
    {
        std::erase_if(delegates_, [=](const KtDelegate<Args...>& delegate)
            {
                return delegate.GetIsSameInstance(instance);
            }
        );
    }

    void Broadcast(Args... args)
    {
        for (const KtDelegate<Args...>& delegate : delegates_)
        {
            delegate.Callback(args...);
        }
    }

    void ClearListeners()
    {
        delegates_ = {};
    }

private:
    std::unordered_set<KtDelegate<Args...>> delegates_;
};