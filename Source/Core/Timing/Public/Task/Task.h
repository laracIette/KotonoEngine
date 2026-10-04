#pragma once
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
struct UTask final
{
	b8 isPlaying;
	f32 duration;
	UEvent<f32> eventUpdate;
	UEvent<> eventCompleted;

	void Update(f32 deltaTime);

	void Start();
	void Stop();

private:
	f32 current_;
};

