#include "Event.h"

void KtEvent::Broadcast()
{
    for (const Listener& listener : _listeners)
    {
        listener.Callback();
    }
}
