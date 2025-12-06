#pragma once
#include <kotono_common/Average.h>
#include <kotono_timing/TimeContext.h>
class STimeManager final
{
	friend class SEngine;

private:
	void Init();
	void Update();

public:
	KtTimeContext& GameTime();
	KtTimeContext& RenderTime();

	float AverageGameTime() const;
	float AverageRenderTime() const;

private:
	KtTimeContext gameTime_;
	KtTimeContext renderTime_;

	KtAverage<256> averageGameTime_;
	KtAverage<64> averageRenderTime_;
};

inline STimeManager TimeManager;
