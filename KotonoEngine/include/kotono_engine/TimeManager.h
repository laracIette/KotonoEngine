#pragma once
#include <kotono_framework/Average.h>
#include <kotono_framework/TimeContext.h>
class STimeManager final
{
	friend class SEngine;

private:
	void Init();
	void Update();

public:
	KtTimeContext& GameTime();

	float AverageGameTime() const;

private:
	KtTimeContext gameTime_;

	KtAverage<256> averageGameTime_;
};