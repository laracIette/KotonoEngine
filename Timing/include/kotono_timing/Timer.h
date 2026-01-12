#pragma once
#include <kotono_common/Event.h>
class KtTimer final
{
	friend class KtTimerManager;

public:
	bool GetIsPlaying() const;
	bool GetIsRepeat() const;
	float GetDuration() const;
	UEvent<>& EventCompleted();

	void SetIsRepeat(const bool isRepeat);
	void SetDuration(const float duration);

	void Start(const bool isOverride = true);
	void Stop();

private:
	void Update(const float deltaTime);

private:
	bool isPlaying_;
	bool isRepeat_;
	float duration_;
	float current_;
	UEvent<> eventCompleted_;
	size index_;
};