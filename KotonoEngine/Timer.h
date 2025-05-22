#pragma once
#include "Object.h"
#include <kotono_framework/Event.h>
class KTimer : public KObject
{
	BASECLASS(KObject)

public:
	void Update() override;

	const bool GetIsRepeat() const;
	const float GetDuration() const;
	KtEvent<>& GetEventCompleted();

	void SetIsRepeat(const bool isRepeat);
	void SetDuration(const float duration);

	void Start();
	void Stop();

private:
	bool isPlaying_;
	bool isRepeat_;
	float startTime_;
	float targetDuration_;
	float currentDuration_;
	KtEvent<> eventCompleted_;
};

