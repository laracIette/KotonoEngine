#pragma once
#include <kotono_common/Average.h>
#include <kotono_common/types.h>
#include <kotono_timing/TimeContext.h>
class GTimeManager final
{
public:
	void Init();
	void Update();

	f32 Now() const;
	f32 Delta() const;

	UTimeContext& GameTime();
	UTimeContext& RenderTime();

	f32 AverageUpdateTime() const;
	f32 AverageGameTime() const;
	f32 AverageRenderTime() const;

private:
	f32 now_;
	f32 delta_;

	UTimeContext gameTime_;
	UTimeContext renderTime_;

	KtAverage<256> averageUpdateTime_;
	KtAverage<256> averageGameTime_;
	KtAverage<256> averageRenderTime_;
};

inline GTimeManager TimeManager;
