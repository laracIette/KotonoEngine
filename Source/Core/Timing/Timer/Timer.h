#pragma once
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
class UTimer final
{
public:
	void Update(f32 deltaTime);

	void Start(b8 isOverride = true);
	void Stop();

	auto GetIsPlaying() const -> b8 { return isPlaying_; }
	auto GetIsRepeat() const -> b8 { return isRepeat_; }
	auto GetDuration() const -> f32 { return duration_; }
	auto EventCompleted() -> UEvent<>& { return eventCompleted_; }

	void SetIsRepeat(b8 isRepeat) { isRepeat_ = isRepeat; }
	void SetDuration(f32 duration) { duration_ = duration; }

private:
	b8 isPlaying_;
	b8 isRepeat_;
	f32 duration_;
	f32 current_;
	UEvent<> eventCompleted_;
	size index_;
};