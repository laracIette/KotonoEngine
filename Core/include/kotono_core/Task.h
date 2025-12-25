#pragma once
#include <kotono_common/Event.h>
struct UTask final
{
	bool isPlaying;
	float duration;
	KtEvent<> eventUpdate;
	KtEvent<> eventCompleted;

	void Update(const float delta);

	void Start();
	void Stop();

private:
	float current_;
};

