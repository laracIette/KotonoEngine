#pragma once
#include "Timer.h"
#include <kotono_common/types.h>
#include <string_view>
#include <unordered_map>
class GTimerManager final
{
	friend class GTimeManager;

public:
	UTimer& GetTimer(const std::string_view name);

private:
	void Update(const f32 deltaTime);

private:
	std::unordered_map<std::string_view, UTimer> timers_;
};

inline GTimerManager TimerManager;
