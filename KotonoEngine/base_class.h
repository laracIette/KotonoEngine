#pragma once
#define BASECLASS(type) \
public: \
template<class Tfunc, typename... Args> \
void ListenEvent(KtEvent<Args...>& event, void (Tfunc::* function)(Args...)) \
{ \
	KObject::ListenEvent(event, this, function); \
} \
private: \
	typedef type Base;