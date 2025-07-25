#pragma once
#include "Object.h"
#include <kotono_framework/Event.h>
class KTimer : public KObject
{
	BASECLASS(KObject)

protected:
	void Update() override;

public:
	const bool GetIsPlaying() const;
	const bool GetIsRepeat() const;
	const float GetDuration() const;
	KtEvent<>& GetEventCompleted();

	void SetIsRepeat(const bool isRepeat);
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

