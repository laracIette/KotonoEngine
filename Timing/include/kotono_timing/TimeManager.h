#pragma once
#include <unordered_map>
#include <string>
#include "Timer.h"
#include <kotono_common/Average.h>
class KtTimeManager final
{
	friend class SEngine;

private:
	void Update();

public:
	KtTimer& GetTimer(const std::string_view name);

	float Now() const;
	float Delta() const;

	float AverageUpdateTime() const;

private:
	float now_;
	float delta_;

	KtAverage<256> updateAverageTime_;

private:
	std::unordered_map<std::string_view, KtTimer> timers_;
};

inline KtTimeManager TimeManager;
