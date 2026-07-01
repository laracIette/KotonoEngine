#pragma once
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
class UTimer final
{
	friend class GTimerManager;

public:
	bool GetIsPlaying() const;
	bool GetIsRepeat() const;
	f32 GetDuration() const;
	UEvent<>& EventCompleted();

	void SetIsRepeat(const bool isRepeat);
	void SetDuration(const f32 duration);

	void Start(const bool isOverride = true);
	void Stop();

private:
	void Update(const f32 deltaTime);

private:
	bool isPlaying_;
	bool isRepeat_;
	f32 duration_;
	f32 current_;
	UEvent<> eventCompleted_;
	size index_;
};