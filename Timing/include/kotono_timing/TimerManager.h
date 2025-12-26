#pragma once
#include <unordered_map>
#include <string>
#include "Timer.h"
class KtTimerManager final
{
	friend class STimeManager;

public:
	KtTimer& GetTimer(const std::string_view name);

private:
	void Update(const float deltaTime);

private:
	std::unordered_map<std::string_view, KtTimer> timers_;
};

inline KtTimerManager TimerManager;
