#pragma once
#include <unordered_map>
#include <string>
#include "Timer.h"
#include "Average.h"
#include "TimeContext.h"
class KtTimeManager final
{
	friend class KtFramework;

private:
	void Init();
	void Update();

public:
	KtTimer& GetTimer(std::string_view name);

	float Now() const;
	float Delta() const;

	KtTimeContext& RenderTime();

	float AverageUpdateTime() const;
	float AverageRenderTime() const;

private:
	float now_;
	float delta_;

	KtTimeContext renderTime_;

	KtAverage<256> updateAverageTime_;
	KtAverage<256> averageRenderTime_;

private:
	std::unordered_map<std::string_view, KtTimer> timers_;
};