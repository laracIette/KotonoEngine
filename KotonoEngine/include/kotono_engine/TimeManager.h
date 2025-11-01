#pragma once
#include <kotono_framework/Average.h>
class STimeManager final
{
	friend class SEngine;

private:
	enum class TimeContextState : char
	{
		Playing,
		Paused
	};

	struct TimeContext
	{
		float frequency;
		float lastDelta;
		float currentDelta;
		float scale;
		float total;
		TimeContextState state;
	};

public:
	void Init();
	void Update();

	float EditorDelta() const;
	float GameDelta() const;
	float RenderDelta() const;

	float EditorNow() const;
	float GameNow() const;
	float RenderNow() const;

	float AverageEngineTime() const;
	float AverageEditorTime() const;
	float AverageGameTime() const;
	float AverageRenderTime() const;

private:
	float currentTime_;
	TimeContext editorTime_;
	TimeContext gameTime_;
	TimeContext renderTime_;

	KtAverage<256> engineAverageTime_;
	KtAverage<256> editorAverageTime_;
	KtAverage<256> gameAverageTime_;
	KtAverage<256> renderAverageTime_;

	/// <summary>
	/// Returns true if the time context reseted, else false
	/// </summary>
	bool UpdateTimeContext(TimeContext& timeContext, const float delta) const;
};