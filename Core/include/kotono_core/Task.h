#pragma once
#include <kotono_common/Event.h>
struct UTask final
{
	bool isPlaying;
	float duration;
	UEvent<f32> eventUpdate;
	UEvent<> eventCompleted;

	void Update(const float deltaTime);

	void Start();
	void Stop();

private:
	float current_;
};

