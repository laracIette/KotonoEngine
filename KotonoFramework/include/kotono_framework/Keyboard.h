#pragma once
#include <map>
#include "Event.h"
#include "Key.h"
class KtKeyboard final
{
public:
	void Update();


	KtEvent& GetEvent(const KtKey key);

private:
	std::map<KtKey, KtEvent> _events;
};

