#pragma once
#include <kotono_common/Average.h>
#include <kotono_timing/TimeContext.h>
class GTimeManager final
{
	friend class GCore;

private:
	void Init();
	void Update();

public:
	float Now() const;
	float Delta() const;

	UTimeContext& GameTime();
	UTimeContext& RenderTime();

	float AverageUpdateTime() const;
	float AverageGameTime() const;
	float AverageRenderTime() const;

private:
	float now_;
	float delta_;

	UTimeContext gameTime_;
	UTimeContext renderTime_;

	KtAverage<256> averageUpdateTime_;
	KtAverage<256> averageGameTime_;
	KtAverage<256> averageRenderTime_;
};

inline GTimeManager TimeManager;
