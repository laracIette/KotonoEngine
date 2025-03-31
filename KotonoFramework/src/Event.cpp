#include "Event.h"
#include "log.h"
#include <iostream>

void KtEvent::Broadcast()
{
    for (const Listener& listener : _listeners)
    {
        if (listener.Instance)
        {
            listener.Callback();
        }
        else
        {
            std::cerr << "can't call listener.Callback(), listener.Instance is nullptr" << std::endl;
        }
    }
}
