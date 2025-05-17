#pragma once
#include <map>
#include <unordered_set>
#include "Event.h"
#include "Key.h"
#include "InputState.h"
class KtKeyboard final
{
public:
	void Update();

	KtEvent<>& GetEvent(const KtKey key, const KtInputState inputState);

private:
	std::map<KtKey, std::map<KtInputState, KtEvent<>>> events_;
	std::map<KtKey, std::unordered_set<KtInputState>> keyStates_;
};

