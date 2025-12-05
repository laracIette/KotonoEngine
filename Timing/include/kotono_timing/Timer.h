#pragma once
#include <kotono_common/Event.h>
class KtTimer final
{
	friend class KtTimeManager;

private:
	void Update(const float delta);

public:
	bool GetIsPlaying() const;
	bool GetIsRepeat() const;
	float GetDuration() const;
	KtEvent<>& EventCompleted();

	void SetIsRepeat(const bool isRepeat);
	void SetDuration(const float duration);

	void Start(const bool isOverride = true);
	void Stop();

private:
	bool isPlaying_;
	bool isRepeat_;
	float duration_;
	float current_;
	KtEvent<> eventCompleted_;
	size_t index_;
};