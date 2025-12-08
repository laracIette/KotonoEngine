#pragma once
#include <kotono_common/Average.h>
#include <kotono_timing/TimeContext.h>
class STimeManager final
{
	friend class SCore;

private:
	void Init();
	void Update();

public:
	float Now() const;
	float Delta() const;

	KtTimeContext& GameTime();
	KtTimeContext& RenderTime();

	float AverageUpdateTime() const;
	float AverageGameTime() const;
	float AverageRenderTime() const;

private:
	float now_;
	float delta_;

	KtTimeContext gameTime_;
	KtTimeContext renderTime_;

	KtAverage<256> averageUpdateTime_;
	KtAverage<256> averageGameTime_;
	KtAverage<256> averageRenderTime_;
};

inline STimeManager TimeManager;
