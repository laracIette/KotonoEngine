#pragma once
#include <kotono_common/Event.h>
struct UTimer
{
	bool isPlaying;
	bool isRepeat;
	float duration;
	KtEvent<> eventCompleted;

	void Update();

	void Start(const bool isOverride = true);
	void Stop();

private:
	float current_;
};

