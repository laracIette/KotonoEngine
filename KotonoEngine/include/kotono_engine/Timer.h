#pragma once
#include "Object.h"
#include <kotono_framework/Event.h>
class KTimer : public KObject
{
	BASECLASS(KObject)

protected:
	void Update() override;

public:
	bool GetIsPlaying() const;
	bool GetIsRepeat() const;
	float GetDuration() const;
	KtEvent<>& GetEventCompleted();

	void SetIsRepeat(const bool isRepeat);
	// Set the duration of the delay, executes next update if duration is 0
	void SetDuration(const float duration);

	void Start(const bool isOverride = true);
	void Stop();

private:
	bool isPlaying_;
	bool isRepeat_;
	float startTime_;
	float targetDuration_;
	float currentDuration_;
	KtEvent<> eventCompleted_;
};

