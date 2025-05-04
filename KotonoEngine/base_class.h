#pragma once
#define BASECLASS(type) typedef type Base; \
public: \
template<class Tfunc, typename... Args> \
void ListenEvent(KtEvent<Args...>& event, void (Tfunc::* function)(Args...)) \
{ \
	OObject::ListenEvent(event, this, function); \
} \
private: