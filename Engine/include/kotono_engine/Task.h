#pragma once
#include <kotono_framework/Event.h>
struct UTask
{
	bool isPlaying;
	float duration;
	KtEvent<> eventUpdate;
	KtEvent<> eventCompleted;

	void Update();

	void Start();
	void Stop();

private:
	float current_;
};

